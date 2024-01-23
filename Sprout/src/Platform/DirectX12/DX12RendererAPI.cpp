#include "spch.h"
#include "DX12RendererAPI.h"

#include "DX12Context.h"

namespace Sprout
{
	void DX12RendererAPI::Init()
	{
		//DX12Context::Get().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void DX12RendererAPI::SetClearColor(const glm::vec4& color)
	{
		ClearColor[0] = color.r;
		ClearColor[1] = color.g;
		ClearColor[2] = color.b;
		ClearColor[3] = color.a;
	}

	void DX12RendererAPI::Clear()
	{
		DX12Context context = DX12Context::Get();
		//context.GetDeviceContext()->ClearRenderTargetView(context.GetRenderTargetView(), ClearColor);
	}

	void DX12RendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vArray)
	{
		vArray->GetIndexBuffer()->Bind();
		std::vector<std::shared_ptr<VertexBuffer>> buffers = vArray->GetVertexBuffers();
		for (std::shared_ptr<VertexBuffer> buffer : buffers)
		{
			buffer->Bind();
			//DX12Context::Get().GetDeviceContext()->DrawIndexed(vArray->GetIndexBuffer()->GetCount(), 0, 0);
		}
	}
}