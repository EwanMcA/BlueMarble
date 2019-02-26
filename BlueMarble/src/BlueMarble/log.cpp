#include "bmpch.h"
#include "log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace BlueMarble {

	std::shared_ptr<spdlog::logger> Log::cCoreLogger;
	std::shared_ptr<spdlog::logger> Log::cClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		cCoreLogger = spdlog::stdout_color_mt("BLUEMARBLE");
		cCoreLogger->set_level(spdlog::level::trace);

		cClientLogger = spdlog::stdout_color_mt("APP");
		cClientLogger->set_level(spdlog::level::trace);

	}
}