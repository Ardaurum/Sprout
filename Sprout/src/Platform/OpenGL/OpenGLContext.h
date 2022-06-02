#pragma once
#include "Sprout/Renderer/RendererContext.h"

#include <GLFW/glfw3.h>

namespace Sprout
{
	class OpenGLContext : public RendererContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* WindowHandle;
	};
}

