#include "spch.h"
#include "OpenGLImGuiBackend.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

namespace Sprout
{
	void OpenGLImGuiBackend::Init(const Window& window)
	{
		GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);
		ImGui_ImplOpenGL3_Init("#version 430");
	}

	void OpenGLImGuiBackend::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
	}

	void OpenGLImGuiBackend::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
	}

	void OpenGLImGuiBackend::RenderDrawData()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
