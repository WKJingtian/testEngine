#pragma once
#include "buffer.h"

namespace tengine
{
	class vArray
	{
	public:
		virtual ~vArray() {}
		virtual void bind() const = 0;
		virtual void unbind() const = 0;
		virtual void addVB(const std::shared_ptr<vBuffer> vb) = 0;
		virtual void addIB(const std::shared_ptr<iBuffer> ib) = 0;
		inline std::vector<std::shared_ptr<vBuffer>> getVBlist() { return vbList; }
		inline std::shared_ptr<vBuffer> getVB(uint32_t ind) { return vbList[ind]; }
		inline std::shared_ptr<iBuffer> getIB() { return ib; }
		static vArray* create();
	protected:
		std::vector<std::shared_ptr<vBuffer>> vbList;
		std::shared_ptr<iBuffer> ib;
	};
}