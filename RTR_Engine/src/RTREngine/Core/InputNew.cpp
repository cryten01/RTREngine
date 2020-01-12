#include "rtrpch.h"
#include "RTREngine/Core/InputNew.h"

#ifdef RTR_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace RTREngine {

	Scope<InputNew> InputNew::s_Instance = InputNew::Create();

	Scope<InputNew> InputNew::Create()
	{
#ifdef RTR_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		RTR_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}