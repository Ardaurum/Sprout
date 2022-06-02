#pragma once

#include "Sprout\Core\EnumUtils.h"

namespace Sprout
{
	enum class AxisOptions : uint8_t
	{
		None = 0,
		X = 1 << 0,
		Y = 1 << 1,
		Z = 1 << 2,

		Right = X,
		Up = Y,
		Forward = Z
	};

	MAKE_ENUM_FLAG(AxisOptions)

	enum class RotationOptions : uint8_t
	{
		None = 0,
		Yaw = 1 << 0,
		Pitch = 1 << 1,
		Roll = 1 << 2
	};

	MAKE_ENUM_FLAG(RotationOptions)
}