#include "spch.h"
#include "RendererContext.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DirectX11/DX11Context.h"

namespace Sprout
{
    std::shared_ptr<RendererContext> RendererContext::Create(GLFWwindow* windowHandle)
    {
        switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLContext>(windowHandle);
		case RendererAPI::API::DirectX11:
			return std::make_shared<DX11Context>(windowHandle);
		}

		SPROUT_CORE_ASSERT(false, "Unknown Renderer API!");
		return nullptr;
    }
}
