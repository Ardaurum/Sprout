#pragma once

#include "MouseCode.h"
#include "KeyCode.h"

#include <glm/fwd.hpp>

namespace Sprout
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode btn);
		static glm::vec2 GetMousePos();
		static float GetMouseX();
		static float GetMouseY();
	};
}