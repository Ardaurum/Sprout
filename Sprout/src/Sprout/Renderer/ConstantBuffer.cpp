#include "spch.h"
#include "ConstantBuffer.h"
#include "Renderer.h"

#include "Platform/DirectX11/DX11ConstantBuffer.h"
#include "Platform/OpenGL/OpenGLConstantBuffer.h"

namespace Sprout
{
	std::shared_ptr<ConstantBuffer> ConstantBuffer::Create(ShaderType applyTo, uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLConstantBuffer>(size, binding);
		case RendererAPI::API::DirectX11:
			return std::make_shared<DX11ConstantBuffer>(applyTo, size, binding);
		}

		SPROUT_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
	}
}