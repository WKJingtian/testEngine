#pragma once
#include "tengine_core.h"
#include <iostream>

namespace tengine
{
	class input
	{
	public:
		virtual ~input() = default;

		static bool isKeyPressed(int keycode) { return input_instance->isKeyPressedImpl(keycode); }
		static bool isButtonPressed(uint32_t button) { return input_instance->isButtonPressedImpl(button); }
		static float getmx() { return input_instance->getmxImpl(); }
		static float getmy() { return input_instance->getmyImpl(); }
		static std::pair<float, float> getmPos() { return input_instance->getmPosImpl(); }
	protected:
		input() = default;
		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isButtonPressedImpl(uint32_t button) = 0;
		virtual float getmxImpl() = 0;
		virtual float getmyImpl() = 0;
		virtual std::pair<float, float> getmPosImpl() = 0;
	private:
		static ownedPtr<input> input_instance;
	};

	class winInput : public input
	{
	protected:
		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isButtonPressedImpl(uint32_t keycode) override;
		virtual float getmxImpl() override;
		virtual float getmyImpl() override;
		virtual std::pair<float, float> getmPosImpl() override;
	};
}