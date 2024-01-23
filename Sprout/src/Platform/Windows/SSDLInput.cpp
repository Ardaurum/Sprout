#include "spch.h"
#include "Sprout/Input/Input.h"

#include "Sprout/Core/App.h"
#include "SDL3/SDL.h"

namespace Sprout
{
	bool Input::IsKeyPressed(KeyCode key)
	{
		uint8_t const* keyboardState = SDL_GetKeyboardState(nullptr);
		return keyboardState[static_cast<uint32_t>(key)] != 0;
	}

	bool Input::IsMouseButtonPressed(MouseCode btn)
	{
		uint32_t mouseState = SDL_GetMouseState(nullptr, nullptr);
		return (mouseState & (1 << (static_cast<uint32_t>(btn) - 1))) != 0;
	}

	glm::vec2 Input::GetMousePos()
	{
		float x, y;
		uint32_t mouseState = SDL_GetMouseState(&x, &y);
		return glm::vec2(x, y);
	}

	float Input::GetMouseX()
	{
		return GetMousePos().x;
	}

	float Input::GetMouseY()
	{
		return GetMousePos().y;
	}
}