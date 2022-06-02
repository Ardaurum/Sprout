#pragma once

#include "Sprout/Renderer/VertexArray.h"

#include <vector>

namespace Sprout
{
	class DX11VertexArray : public VertexArray
	{
	public:
		DX11VertexArray() {};
		virtual ~DX11VertexArray() {}
		
		virtual void Bind() const override {};
		virtual void Unbind() const override {};

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& buffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return VBuffer; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return IBuffer; }

	private:
		std::vector<std::shared_ptr<VertexBuffer>> VBuffer;
		std::shared_ptr<IndexBuffer> IBuffer;
	};
}