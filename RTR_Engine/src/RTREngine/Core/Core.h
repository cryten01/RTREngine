#pragma once

#include <memory>

#ifdef RTR_PLATFORM_WINDOWS
	#ifdef RTR_BUILD_DLL
		#define RTR_API __declspec(dllexport)
	#else
		#define RTR_API __declspec(dllimport)
	#endif
#else
	#error RTR_Engine only supports Windows!
#endif

#ifdef RTR_DEBUG
	#define RTR_ENABLE_ASSERTS
#endif

#ifdef RTR_ENABLE_ASSERTS
	#define RTR_ASSERT(x, ...) { if(!(x)) { RTR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RTR_CORE_ASSERT(x, ...) { if(!(x)) { RTR_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RTR_ASSERT(x, ...)
	#define RTR_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define RTR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


namespace RTREngine {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}