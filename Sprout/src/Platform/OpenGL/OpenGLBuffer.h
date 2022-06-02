#pragma once

#include "Sprout/Renderer/Buffer.h"
#include "OpenGLInputLayout.h"

namespace Sprout
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		
		virtual void SetLayout(const LayoutDataType layoutType) override { Layout = std::make_shared<OpenGLInputLayout>(layoutType); }
		virtual const std::shared_ptr<InputLayout>& GetLayout() const override { return Layout; }

	private:
		uint32_t RendererID;
		std::shared_ptr<OpenGLInputLayout> Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const { return Count; }

	private:
		uint32_t RendererID;
		uint32_t Count;
	};
}