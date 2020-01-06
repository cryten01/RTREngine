#pragma once

#ifdef RTR_PLATFORM_WINDOWS
	#ifdef RTR_BUILD_DLL
		#define RTR_API __declspec(dllexport)
	#else
		#define RTR_API __declspec(dllimport)
	#endif // RTR_BUILD_DLL
#else
	#error RTR_Engine only supports Windows!
#endif // RTR_PLATFORM_WINDOWS
