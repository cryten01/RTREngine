#include "rtrpch.h"
#include "RTREngine/Core/Window.h"

#ifdef RTR_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace RTREngine
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef RTR_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		RTR_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}