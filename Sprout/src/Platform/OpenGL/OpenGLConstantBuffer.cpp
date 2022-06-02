#include "spch.h"
#include "OpenGLConstantBuffer.h"

namespace Sprout
{
	OpenGLConstantBuffer::OpenGLConstantBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &RendererID);
		glNamedBufferData(RendererID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, RendererID);
	}

	OpenGLConstantBuffer::~OpenGLConstantBuffer()
	{
		glDeleteBuffers(1, &RendererID);
	}

	void OpenGLConstantBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(RendererID, offset, size, data);
	}
}
