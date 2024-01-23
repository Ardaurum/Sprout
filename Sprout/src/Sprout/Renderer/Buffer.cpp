#include "spch.h"
#include "Buffer.h"
#include "Renderer.h"

#include "Platform/DirectX11/DX11Buffer.h"

namespace Sprout
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::Direct3D11:
			return std::make_shared<DX11VertexBuffer>(vertices, size);
		}

		SPROUT_CORE_FATAL("Unknown Renderer API!");
		return nullptr;
	}
	
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::Direct3D11:
			return std::make_shared<DX11IndexBuffer>(indices, count);
		}

		SPROUT_CORE_FATAL("Unknown Renderer API!");
		return nullptr;
	}
}