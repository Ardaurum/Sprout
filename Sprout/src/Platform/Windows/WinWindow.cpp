#include "spch.h"
#include "WinWindow.h"

#include "Sprout/CoreEvents/WinEvent.h"
#include "Sprout/CoreEvents/KeyEvent.h"
#include "Sprout/CoreEvents/MouseEvent.h"

#include "Sprout/Renderer/RendererContext.h"
#include "Sprout/Renderer/RendererAPI.h"

namespace Sprout
{
	std::shared_ptr<Window> Window::Create(const WindowProperties& Properties)
	{
		return std::make_shared<WinWindow>(Properties);
	}

	static uint8_t glfwWindowCount = 0;

	static void GLFWErrorCallback(int error_code, const char* description)
	{
		SPROUT_CORE_ERROR("GLFW Error: {0}. Description: {1}", error_code, description);
	}

	WinWindow::WinWindow(const WindowProperties& properties)
	{
		SPROUT_CORE_INFO("Creating window \"{0}\": {1}, {2}", properties.Title, properties.Width, properties.Height);

		WinData.Title = properties.Title + ": " + RendererAPI::CurrentAPI;
		WinData.Height = properties.Height;
		WinData.Width = properties.Width;

		if (glfwWindowCount == 0)
		{
			int success = glfwInit();
			SPROUT_CORE_ASSERT(success, "Failed to initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		glfwWindowCount++;

		if (RendererAPI::GetAPI() != RendererAPI::API::OpenGL)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		Window = glfwCreateWindow(properties.Width, properties.Height, properties.Title.c_str(), nullptr, nullptr);

		Context = RendererContext::Create(Window);
		Context->Init();

		//TODO: Make VSync Renderer specific
		//glfwSwapInterval(1);
		glfwSetWindowUserPointer(Window, &WinData);

		SetupWindowEvents();
		SetupMouseEvents();
		SetupKeyEvents();
	}

	WinWindow::~WinWindow()
	{
		Shutdown();
	}

	void WinWindow::Update()
	{
		glfwPollEvents();
		Context->SwapBuffers();
	}

	void WinWindow::SetupWindowEvents()
	{
		glfwSetWindowCloseCallback(Window, [](GLFWwindow* window) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			data->EventCallback(WinCloseEvent());
		});

		glfwSetWindowSizeCallback(Window, [](GLFWwindow* window, int width, int height) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			data->Width = width;
			data->Height = height;

			data->EventCallback(WinResizeEvent(width, height));
		});

		glfwSetWindowIconifyCallback(Window, [](GLFWwindow* window, int status) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			data->IsMinimized = status == GLFW_TRUE;
			if (data->IsMinimized)
			{
				data->EventCallback(WinMinimizeEvent());
			}
			else
			{
				data->EventCallback(WinRestoreEvent());
			}
		});

		glfwSetWindowMaximizeCallback(Window, [](GLFWwindow* window, int status) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			data->EventCallback(WinMaximizeEvent(status == GLFW_TRUE));
		});

		glfwSetWindowFocusCallback(Window, [](GLFWwindow* window, int status) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			if (status == GLFW_TRUE)
			{
				data->EventCallback(WinFocusEvent());
			}
			else
			{
				data->EventCallback(WinFocusLostEvent());
			}
		});

		glfwSetWindowPosCallback(Window, [](GLFWwindow* window, int x, int y) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			data->EventCallback(WinMoveEvent(x, y));
		});
	}

	void WinWindow::SetupMouseEvents()
	{
		glfwSetCursorPosCallback(Window, [](GLFWwindow* window, double x, double y) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			data->EventCallback(MouseMoveEvent(static_cast<float>(x), static_cast<float>(y)));
		});

		glfwSetMouseButtonCallback(Window, [](GLFWwindow* window, int button, int action, int mods) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			MouseCode code = static_cast<MouseCode>(button);
			switch(action)
			{
			case GLFW_PRESS:
				data->EventCallback(MouseBtnDownEvent(code));
				break;
			case GLFW_RELEASE:
				data->EventCallback(MouseBtnUpEvent(code));
				break;
			}
		});

		glfwSetScrollCallback(Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			data->EventCallback(MouseScrollEvent(static_cast<float>(xOffset), static_cast<float>(yOffset)));
		});
	}

	void WinWindow::SetupKeyEvents()
	{
		glfwSetKeyCallback(Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			KeyCode keyCode = static_cast<KeyCode>(key);
			KeyMod keyMod = static_cast<KeyMod>(mods);
			switch (action)
			{
			case GLFW_PRESS:
				data->EventCallback(KeyDownEvent(keyCode, keyMod));
				break;
			case GLFW_RELEASE:
				data->EventCallback(KeyUpEvent(keyCode, keyMod));
				break;
			case GLFW_REPEAT:
				data->EventCallback(KeyRepeatEvent(keyCode, keyMod));
				break;
			}
		});

		glfwSetCharCallback(Window, [](GLFWwindow* window, unsigned int codepoint) {
			Data* data = static_cast<Data*>(glfwGetWindowUserPointer(window));
			KeyCode keyCode = static_cast<KeyCode>(codepoint);
			data->EventCallback(KeyTypeEvent(keyCode));
		});
	}

	void WinWindow::Shutdown()
	{
		glfwDestroyWindow(Window);
		glfwWindowCount--;

		if (glfwWindowCount == 0)
		{
			glfwTerminate();
		}
	}
}