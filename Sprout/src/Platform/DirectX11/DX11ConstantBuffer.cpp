#include "spch.h"
#include "DX11ConstantBuffer.h"
#include "DX11Context.h"

namespace Sprout
{
	DX11ConstantBuffer::DX11ConstantBuffer(ShaderType applyTo, uint32_t size, uint32_t binding)
	{
		uint32_t padding = size % 16;
		if (padding != 0)
		{
			size += 16 - padding;
		}

		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cBufferDesc.ByteWidth = size;
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;

		ApplyShaders = applyTo;
		Slot = binding;

		HRESULT result = DX11Context::Get().GetDevice()->CreateBuffer(&cBufferDesc, nullptr, BufferHandle.GetAddressOf());
		SPROUT_CORE_ASSERT(SUCCEEDED(result), "Failed to create a constant buffer!");
	}

	void DX11ConstantBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		HRESULT result = DX11Context::Get().GetDeviceContext()->Map(BufferHandle.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		SPROUT_CORE_ASSERT(SUCCEEDED(result), "Failed to map constant buffer resource!");

		memcpy((char*) resource.pData + offset, data, size);
		DX11Context::Get().GetDeviceContext()->Unmap(BufferHandle.Get(), 0);

		if (Any(ApplyShaders & ShaderType::Vertex))
		{
			DX11Context::Get().GetDeviceContext()->VSSetConstantBuffers(Slot, 1, BufferHandle.GetAddressOf());
		}
		if (Any(ApplyShaders & ShaderType::Pixel))
		{
			DX11Context::Get().GetDeviceContext()->PSSetConstantBuffers(Slot, 1, BufferHandle.GetAddressOf());
		}
	}
}
