#pragma once

#include <GLFW/glfw3.h>

namespace Sprout
{
	class RendererContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static std::shared_ptr<RendererContext> Create(GLFWwindow* windowHandle);
	};
}
