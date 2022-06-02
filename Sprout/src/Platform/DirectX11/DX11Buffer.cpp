#include "spch.h"
#include "DX11Buffer.h"

#include "DX11Context.h"
#include "DX11Shader.h"

#include <d3d11.h>

namespace Sprout
{
	// ======================== Vertex Buffer ========================

	DX11VertexBuffer::DX11VertexBuffer(float* vertices, uint32_t size)
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferInitData;
		bufferInitData.pSysMem = vertices;
		HRESULT result = DX11Context::Get().GetDevice()->CreateBuffer(&bufferDesc, &bufferInitData, BufferHandle.GetAddressOf());
		SPROUT_CORE_ASSERT(SUCCEEDED(result), "Creating vertex buffer failed!");
	}

	DX11VertexBuffer::~DX11VertexBuffer()
	{
	}

	void DX11VertexBuffer::Bind() const
	{
		Layout->Bind();
		uint32_t offset = 0;
		uint32_t stride = Layout->GetDesc().GetStride();
		DX11Context::Get().GetDeviceContext()->IASetVertexBuffers(0, 1, BufferHandle.GetAddressOf(), &stride, &offset);
	}

	void DX11VertexBuffer::Unbind() const
	{
	}

	// ======================== Index Buffer ========================

	DX11IndexBuffer::DX11IndexBuffer(uint32_t* indices, uint32_t count)
		: Count(count)
	{
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = sizeof(uint32_t) * count;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferInitData;
		bufferInitData.pSysMem = indices;
		HRESULT result = DX11Context::Get().GetDevice()->CreateBuffer(&bufferDesc, &bufferInitData, BufferHandle.GetAddressOf());
		SPROUT_CORE_ASSERT(SUCCEEDED(result), "Creating index buffer failed!");
	}

	DX11IndexBuffer::~DX11IndexBuffer()
	{
	}

	void DX11IndexBuffer::Bind() const
	{
		DX11Context::Get().GetDeviceContext()->IASetIndexBuffer(BufferHandle.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
	
	void DX11IndexBuffer::Unbind() const
	{
	}
}
