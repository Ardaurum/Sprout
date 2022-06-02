#include "spch.h"

#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include <iostream>

namespace Sprout 
{
	std::shared_ptr<spdlog::logger> Log::CoreLogger;
	std::shared_ptr<spdlog::logger> Log::ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] (%n - %l): %v%$");
		CoreLogger = spdlog::stdout_color_mt("SPROUT");
		CoreLogger->set_level(spdlog::level::level_enum::trace);

		ClientLogger = spdlog::stdout_color_mt("APP");
		ClientLogger->set_level(spdlog::level::level_enum::trace);
	}
}