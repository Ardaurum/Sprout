#pragma once

#include "Sprout/Core/Core.h"
#include <spdlog/spdlog.h>

namespace Sprout
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> CoreLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;
	};
}

#define SPROUT_CORE_TRACE(...) ::Sprout::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define SPROUT_CORE_INFO(...) ::Sprout::Log::GetCoreLogger()->info(__VA_ARGS__);
#define SPROUT_CORE_WARN(...) ::Sprout::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define SPROUT_CORE_ERROR(...) ::Sprout::Log::GetCoreLogger()->error(__VA_ARGS__);
#define SPROUT_CORE_FATAL(...) ::Sprout::Log::GetCoreLogger()->critical(__VA_ARGS__);

#define SPROUT_TRACE(...) ::Sprout::Log::GetClientLogger()->trace(__VA_ARGS__);
#define SPROUT_INFO(...) ::Sprout::Log::GetClientLogger()->info(__VA_ARGS__);
#define SPROUT_WARN(...) ::Sprout::Log::GetClientLogger()->warn(__VA_ARGS__);
#define SPROUT_ERROR(...) ::Sprout::Log::GetClientLogger()->error(__VA_ARGS__);
#define SPROUT_FATAL(...) ::Sprout::Log::GetClientLogger()->critical(__VA_ARGS__);