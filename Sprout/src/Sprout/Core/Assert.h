#pragma once

#include "Log.h"
#include <filesystem>

#ifdef SPROUT_ASSERTS_ENABLE
	#define SPROUT_INTERNAL_ASSERT(type, check, msg, ...) { if (!(check)) { SPROUT##type##FATAL(msg, __VA_ARGS__) SPROUT_DEBUGBREAK(); }}
	#define SPROUT_INTERNAL_ASSERT_MSG(type, check, ...) SPROUT_INTERNAL_ASSERT(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define SPROUT_INTERNAL_ASSERT_NO_MSG(type, check) SPROUT_INTERNAL_ASSERT(type, check, "Assertion '{0}' failed at {1}:{2}", #check, std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define SPROUT_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define SPROUT_INTERNAL_ASSERT_GET_MACRO(...) SPROUT_EXP_MACRO(SPROUT_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, SPROUT_INTERNAL_ASSERT_MSG, SPROUT_INTERNAL_ASSERT_NO_MSG))

	#define SPROUT_ASSERT(...) SPROUT_EXP_MACRO(SPROUT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
	#define SPROUT_CORE_ASSERT(...) SPROUT_EXP_MACRO(SPROUT_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
	#define SPROUT_ASSERT(...)
	#define SPROUT_CORE_ASSERT(...)
#endif