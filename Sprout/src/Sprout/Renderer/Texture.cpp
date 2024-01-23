#include "spch.h"
#include "Texture.h"

#include "Renderer.h"

#include "Platform/DirectX11/DX11Texture.h"

namespace Sprout
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::Direct3D11:
			return std::make_shared<DX11Texture2D>(path);
		}

		SPROUT_CORE_FATAL("Unknown Renderer API!");
		return nullptr;
	}
}