#pragma once

namespace tengine
{
	class texture
	{
	public:
		virtual ~texture() {}

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual void setData(void* data, uint32_t size) = 0;
		virtual bool operator==(const texture& other) = 0;

		virtual void bind(uint32_t slot = 0) const = 0;
		uint32_t renID;
		std::string src;
	};

	class texture2D : public texture
	{
	public:
		static std::shared_ptr<texture2D> create(const char* texPath);
	};
}