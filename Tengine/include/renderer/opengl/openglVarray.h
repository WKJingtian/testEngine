#pragma once
#include "../vertexArray.h"

namespace tengine
{
	class openglVarray : public vArray
	{
	public:
		openglVarray();
		virtual ~openglVarray();
		virtual void bind() const override;
		virtual void unbind() const override;
		virtual void addVB(const std::shared_ptr<vBuffer> vb) override;
		virtual void addIB(const std::shared_ptr<iBuffer> ib) override;
	private:
		uint32_t renID;
	};
}