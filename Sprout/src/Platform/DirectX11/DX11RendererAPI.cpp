#include "spch.h"
#include "DX11RendererAPI.h"

#include "DX11Context.h"
#include "DX11Buffer.h"

#include "Sprout/Core/App.h"

namespace Sprout
{
	void DX11RendererAPI::Init()
	{
		DX11Context::Get().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void DX11RendererAPI::SetClearColor(const glm::vec4& color)
	{
		ClearColor[0] = color.r;
		ClearColor[1] = color.g;
		ClearColor[2] = color.b;
		ClearColor[3] = color.a;
	}

	void DX11RendererAPI::Clear()
	{
		DX11Context context = DX11Context::Get();
		context.GetDeviceContext()->ClearRenderTargetView(context.GetRenderTargetView(), ClearColor);
	}

	void DX11RendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vArray)
	{
		vArray->GetIndexBuffer()->Bind();
		std::vector<std::shared_ptr<VertexBuffer>> buffers = vArray->GetVertexBuffers();
		for (std::shared_ptr<VertexBuffer> buffer : buffers)
		{
			buffer->Bind();
			DX11Context::Get().GetDeviceContext()->DrawIndexed(vArray->GetIndexBuffer()->GetCount(), 0, 0);
		}
	}
}