#pragma once

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace BlueMarble {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return cCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return cClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> cClientLogger;
		static std::shared_ptr<spdlog::logger> cCoreLogger;
	};

}

// Core Logging macros
#define BM_CORE_ERROR(...) ::BlueMarble::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BM_CORE_WARN(...)  ::BlueMarble::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BM_CORE_INFO(...)  ::BlueMarble::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BM_CORE_TRACE(...) ::BlueMarble::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BM_CORE_FATAL(...) ::BlueMarble::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client Logging macros
#define BM_ERROR(...) ::BlueMarble::Log::GetClientLogger()->error(__VA_ARGS__)
#define BM_WARN(...)  ::BlueMarble::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BM_INFO(...)  ::BlueMarble::Log::GetClientLogger()->info(__VA_ARGS__)
#define BM_TRACE(...) ::BlueMarble::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BM_FATAL(...) ::BlueMarble::Log::GetClientLogger()->fatal(__VA_ARGS__)

// If distribution build.. def these to nothing..