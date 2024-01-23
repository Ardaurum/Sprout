#include "spch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/DirectX11/DX11VertexArray.h"

namespace Sprout
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::Direct3D11:
			return std::make_shared<DX11VertexArray>();
		}

		SPROUT_CORE_FATAL("Unknown Renderer API!");
		return nullptr;
	}
}