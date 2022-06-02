#pragma once

#include "glm/glm.hpp"
#include "KeyCode.h"
#include "MouseCode.h"

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