#include "spch.h"
#include "RenderCommand.h"

#include "Platform/DirectX11/DX11RendererAPI.h"
#include "Platform/FakeRenderer/FakeRendererAPI.h"

namespace Sprout 
{
	std::unique_ptr<RendererAPI> RenderCommand::API = std::make_unique<FakeRendererAPI>();

	void RenderCommand::SetupAPI()
	{
		switch(RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			break;
		case RendererAPI::API::Direct3D11:
			API = std::make_unique<DX11RendererAPI>();
			break;
		default:
			SPROUT_CORE_FATAL("Unknown Renderer API!");
			break;
		}
	}
}