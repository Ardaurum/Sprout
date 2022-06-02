#pragma once

#include "Sprout/Renderer/ConstantBuffer.h"

#include <glad/glad.h>

namespace Sprout
{
	class OpenGLConstantBuffer : public ConstantBuffer
	{
	public:
		OpenGLConstantBuffer(uint32_t size, uint32_t binding);
		virtual ~OpenGLConstantBuffer();

		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

	private:
		uint32_t RendererID;
	};
}