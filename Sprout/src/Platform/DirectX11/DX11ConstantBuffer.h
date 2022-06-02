#pragma once

#include "Sprout/Renderer/ConstantBuffer.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX11ConstantBuffer : public ConstantBuffer
	{
	public:
		DX11ConstantBuffer(ShaderType applyTo, uint32_t size, uint32_t binding);
		virtual ~DX11ConstantBuffer() {}

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> BufferHandle;
		ShaderType ApplyShaders;
		uint32_t Slot;
	};
}