#pragma once

#include "Log.h"
#include "MacroUtils.h"
#include <filesystem>

#ifdef SPROUT_ASSERTS_ENABLE
	#define SPROUT_INTERNAL_ASSERT(type, check, msg, ...) { if (!(check)) { SPROUT##type##ERROR(msg, __VA_ARGS__) SPROUT_DEBUGBREAK(); }}
	#define SPROUT_INTERNAL_ASSERT_MSG(type, check, msg, ...) SPROUT_INTERNAL_ASSERT(type, check, "Assertion failed: {0}. Check '{1}' failed at {2}:{3}", fmt::format(msg, __VA_ARGS__), #check, std::filesystem::path(__FILE__).filename().string(), __LINE__)
	#define SPROUT_INTERNAL_ASSERT_NO_MSG(type, check) SPROUT_INTERNAL_ASSERT(type, check, "Assertion '{0}' failed at {1}:{2}", #check, std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define SPROUT_ASSERT(...) SPROUT_EXP_MACRO(SPROUT_INTERNAL_ASSERT_NO_MSG(_, __VA_ARGS__))
	#define SPROUT_ASSERT_MSG(...) SPROUT_EXP_MACRO(SPROUT_INTERNAL_ASSERT_MSG(_, __VA_ARGS__))
	#define SPROUT_CORE_ASSERT(...) SPROUT_EXP_MACRO(SPROUT_INTERNAL_ASSERT_NO_MSG(_CORE_, __VA_ARGS__))
	#define SPROUT_CORE_ASSERT_MSG(...) SPROUT_EXP_MACRO(SPROUT_INTERNAL_ASSERT_MSG(_CORE_, __VA_ARGS__))
#else
#define SPROUT_ASSERT(...)
#define SPROUT_ASSERT_MSG(...)
#define SPROUT_CORE_ASSERT(...)
#define SPROUT_CORE_ASSERT_MSG(...)
#endif