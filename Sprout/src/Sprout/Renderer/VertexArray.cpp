#include "spch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/DirectX11/DX11VertexArray.h"

namespace Sprout
{
	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		case RendererAPI::API::DirectX11:
			return std::make_shared<DX11VertexArray>();
		}

		SPROUT_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}