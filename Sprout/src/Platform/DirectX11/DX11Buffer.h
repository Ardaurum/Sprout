#pragma once

#include "Sprout/Renderer/Buffer.h"
#include "Sprout/Renderer/Shader.h"
#include "DX11InputLayout.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX11VertexBuffer : public VertexBuffer
	{
	public:
		DX11VertexBuffer(float* vertices, uint32_t size);
		virtual ~DX11VertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const LayoutDataType layoutType) override 
		{ 
			Layout = std::make_shared<DX11InputLayout>(LayoutDesc(layoutType)); 
		}

		virtual const std::shared_ptr<InputLayout> GetLayout() const override { return Layout; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> BufferHandle;
		std::shared_ptr<DX11InputLayout> Layout;
	};

	class DX11IndexBuffer : public IndexBuffer
	{
	public:
		DX11IndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~DX11IndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const { return Count; }

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> BufferHandle;
		uint32_t Count;
	};
}