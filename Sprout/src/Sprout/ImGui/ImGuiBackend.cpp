#include "spch.h"
#include "ImGuiBackend.h"

#include "Sprout/Renderer/RendererAPI.h"
#include "Sprout/Core/Window.h"

#include "Platform/DirectX11/DX11ImGuiBackend.h"

namespace Sprout
{
	std::shared_ptr<ImGuiBackend> ImGuiBackend::Create(const RendererAPI::API api)
	{
		switch (api)
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::Direct3D11:
			return std::make_shared<DX11ImGuiBackend>();
		}

		SPROUT_CORE_FATAL("Unknown Renderer API!");
		return nullptr;
	}
}