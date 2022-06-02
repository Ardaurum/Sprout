#include "spch.h"
#include "Texture.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/DirectX11/DX11Texture.h"

namespace Sprout
{
	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		case RendererAPI::API::DirectX11:
			return std::make_shared<DX11Texture2D>(path);
		}

		SPROUT_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}