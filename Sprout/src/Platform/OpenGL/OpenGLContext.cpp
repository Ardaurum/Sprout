#include "spch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>

namespace Sprout
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: WindowHandle(windowHandle)
	{}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		SPROUT_CORE_ASSERT(status, "Failed to initialize Glad!");

		SPROUT_CORE_INFO("OpenGL Info:");
		SPROUT_CORE_INFO("Vendor: {0}", glGetString(GL_VENDOR));
		SPROUT_CORE_INFO("Version: {0}", glGetString(GL_VERSION));
		SPROUT_CORE_INFO("Renderer: {0}", glGetString(GL_RENDERER));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(WindowHandle);
	}
}