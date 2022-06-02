#include "spch.h"
#include "Sprout/Input/Input.h"

#include "Sprout/Core/App.h"
#include <GLFW/glfw3.h>

namespace Sprout
{
	bool Input::IsKeyPressed(KeyCode key)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
		int keyStatus = glfwGetKey(window, static_cast<int>(key));
		return keyStatus == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseCode btn)
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
		int btnStatus = glfwGetMouseButton(window, static_cast<int>(btn));
		return btnStatus == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePos()
	{
		GLFWwindow* window = static_cast<GLFWwindow*>(App::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
	
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