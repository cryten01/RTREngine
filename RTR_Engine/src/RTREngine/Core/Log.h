#pragma once

#include "RTREngine/Core/Core.h"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> // For logging custom types


namespace RTREngine 
{
	class RTR_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

// Core log macros
#define RTR_CORE_TRACE(...)    ::RTREngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RTR_CORE_INFO(...)     ::RTREngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RTR_CORE_WARN(...)     ::RTREngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RTR_CORE_ERROR(...)    ::RTREngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RTR_CORE_CRITICAL(...) ::RTREngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define RTR_CLIENT_TRACE(...)         ::RTREngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RTR_CLIENT_INFO(...)          ::RTREngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define RTR_CLIENT_WARN(...)          ::RTREngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RTR_CLIENT_ERROR(...)         ::RTREngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define RTR_CLIENT_CRITICAL(...)      ::RTREngine::Log::GetClientLogger()->critical(__VA_ARGS__)