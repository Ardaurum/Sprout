#include "spch.h"
#include "RendererContext.h"
#include "Renderer.h"

#include "Platform/DirectX11/DX11Context.h"
#include "Platform/DirectX12/DX12Context.h"

namespace Sprout
{
    std::shared_ptr<RendererContext> RendererContext::Create(Window* windowHandle)
    {
        switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::Direct3D11:
			return std::make_shared<DX11Context>(windowHandle);
		case RendererAPI::API::Direct3D12:
			return std::make_shared<DX12Context>(windowHandle);
		}

		SPROUT_CORE_FATAL("Unknown Renderer API!");
		return nullptr;
    }
}
