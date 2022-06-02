#pragma once

#include "PlatformDetection.h"

#ifdef SPROUT_DEBUG
	#ifdef SPROUT_PLATFORM_WIN
		#define SPROUT_DEBUGBREAK() __debugbreak()
	#elif SPROUT_PLATFORM_LINUX
		#define SPROUT_DEBUGBREAK() raise(SIGTRAP)
	#else
		#define "Platform not supported yet!"
	#endif
	#define SPROUT_ASSERTS_ENABLE
#else
	#define SPROUT_DEBUGBREAK()
#endif

#define SPROUT_EXP_MACRO(x) x

#define STR_TO_BOOL(bool_value) bool_value ? "TRUE" : "FALSE"

#include "Log.h"
#include "Assert.h"