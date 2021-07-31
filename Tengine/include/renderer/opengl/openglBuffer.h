#pragma once
#include "../buffer.h"

namespace tengine
{
	class openglVBuffer : public vBuffer
	{
	public:
		openglVBuffer(float* v, uint32_t s);
		openglVBuffer(uint32_t s);
		virtual ~openglVBuffer();
		virtual void setData(const void* data, uint32_t size) const override;
		virtual void bind()const override;
		virtual void unbind()const override;
	private:
		uint32_t renID;
	};
	class openglIBuffer : public iBuffer
	{
	public:
		openglIBuffer(unsigned int* i, uint32_t c);
		virtual ~openglIBuffer();
		virtual void bind()const  override;
		virtual void unbind()const  override;
		virtual uint32_t getCount() const override { return count; }
	private:
		uint32_t renID;
		uint32_t count;
	};

	class openglFrameBuffer : public frameBuffer
	{
	public:
		openglFrameBuffer(const frameBufferProp& p);
		~openglFrameBuffer();
		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;
		virtual void resize(float x, float y) override;
	};
}