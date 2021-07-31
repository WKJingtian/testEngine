#pragma once
#include "util/utillog.h"
#include <vector>

namespace tengine
{
	enum class shaderDatatype
	{
		None = 0,
		float1, float2, float3, float4,
		mat3, mat4,
		int1, int2, int3, int4, boool
	};

	static uint32_t getSize(shaderDatatype t)
	{
		switch (t)
		{
		case shaderDatatype::float1: return 4*1;
		case shaderDatatype::float2: return 4*2;
		case shaderDatatype::float3: return 4*3;
		case shaderDatatype::float4: return 4*4;
		case shaderDatatype::int1: return 4 * 1;
		case shaderDatatype::int2: return 4 * 2;
		case shaderDatatype::int3: return 4 * 3;
		case shaderDatatype::int4: return 4 * 4;
		case shaderDatatype::mat3: return 4 * 9;
		case shaderDatatype::mat4: return 4 * 16;
		case shaderDatatype::boool: return 1;
		}

		log("unknown shader datatype", 2);
		return 0;
	}

	class bufferElement
	{
	public:
		std::string name;
		shaderDatatype type;
		uint32_t size;
		uint32_t offset;
		bool normalized;
		bufferElement() {}
		bufferElement(const std::string& n, shaderDatatype t, bool nor = false)
			: name(n), type(t), size(getSize(t)), offset(0), normalized(nor)
		{}

		uint32_t getCount() const
		{
			switch (type)
			{
			case shaderDatatype::float1: return 1;
			case shaderDatatype::float2: return 2;
			case shaderDatatype::float3: return 3;
			case shaderDatatype::float4: return 4;
			case shaderDatatype::int1: return 1;
			case shaderDatatype::int2: return 2;
			case shaderDatatype::int3: return 3;
			case shaderDatatype::int4: return 4;
			case shaderDatatype::mat3: return 9;
			case shaderDatatype::mat4: return 16;
			case shaderDatatype::boool: return 1;
			}

			log("unknown shader datatype", 2);
			return 0;
		}
	};

	class bufferLayout
	{
	public:
		bufferLayout() {}
		bufferLayout(const std::vector<bufferElement>& e) : ele(e)
		{ getOffset(); }
		bufferLayout(const std::initializer_list<bufferElement>& e) : ele(e)
		{ getOffset(); }
		inline const std::vector<bufferElement>& getEle() const { return ele; }
		std::vector<bufferElement>::iterator begin() { return ele.begin(); }
		std::vector<bufferElement>::iterator end() { return ele.end(); }
		uint32_t getStride() const { return stride; }
	private:
		void getOffset()
		{
			uint32_t offset = 0;
			stride = 0;
			for (auto& e : ele)
			{
				e.offset = offset;
				offset += e.size;
				stride += e.size;
			}
		}
		std::vector<bufferElement> ele;
		uint32_t stride = 0;
	};

	class vBuffer
	{
	public:
		virtual ~vBuffer() {}
		virtual void setData(const void* data, uint32_t size) const = 0;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		static vBuffer* create(float* v, uint32_t s);
		static vBuffer* create(uint32_t s);
		bufferLayout layout;
	};

	class iBuffer
	{
	public:
		virtual ~iBuffer() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual uint32_t getCount() const = 0;
		static iBuffer* create(unsigned int* i, uint32_t s);
	};

	struct frameBufferProp
	{
		uint32_t width = 1280, height = 720;
		uint32_t sample = 1;
		bool swapChainTarget = false;
	};

	class frameBuffer
	{
	public:
		virtual ~frameBuffer() {}

		frameBufferProp prop;
		uint32_t renID = 0;
		uint32_t colorAttachment = 0, depthAttachment = 0;

		inline const frameBufferProp& getProp() const { return prop; }
		static frameBuffer* create(const frameBufferProp& p);

		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void resize(float x, float y) = 0;
	};
}