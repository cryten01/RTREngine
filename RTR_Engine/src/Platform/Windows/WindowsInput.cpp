#include "rtrpch.h"

#include "Platform/Windows/WindowsInput.h"

#include "RTREngine/Core/App.h"

#include <GLFW/glfw3.h>


namespace RTREngine {

	bool WindowsInput::IsKeyPressedImpl(KeyCode key)
	{
		// TODO
		return true;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(MouseCode button)
	{
		// TODO
		return true;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		// TODO
		return { (float)1.0, (float)1.0 };
	}

	float WindowsInput::GetMouseXImpl()
	{
		// TODO
		return 1.0;
	}

	float WindowsInput::GetMouseYImpl()
	{
		// TODO
		return 1.0;
	}

}