#include "spch.h"
#include "ImGuiBackend.h"

#include "Sprout/Renderer/RendererAPI.h"
#include "Sprout/Core/Window.h"

#include "Platform/OpenGL/OpenGLImGuiBackend.h"
#include "Platform/DirectX11/DX11ImGuiBackend.h"

namespace Sprout
{
	std::shared_ptr<ImGuiBackend> ImGuiBackend::Create(const RendererAPI::API api)
	{
		switch (api)
		{
		case RendererAPI::API::None:
			SPROUT_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLImGuiBackend>();
		case RendererAPI::API::DirectX11:
			return std::make_shared<DX11ImGuiBackend>();
		}

		SPROUT_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}