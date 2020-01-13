#include "rtrpch.h"
#include "RTREngine/Core/Input.h"

#ifdef RTR_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace RTREngine {

	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
		#ifdef RTR_PLATFORM_WINDOWS
				return CreateScope<WindowsInput>();
		#else
				RTR_CORE_ASSERT(false, "Unknown platform!");
				return nullptr;
		#endif
	}
}