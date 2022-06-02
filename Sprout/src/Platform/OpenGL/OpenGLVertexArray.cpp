#include "spch.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include <glad/glad.h>

namespace Sprout
{
	static GLenum LayoutDataTypeToOpenGLBaseType(LayoutDataType type)
	{
		switch(type)
		{
		case LayoutDataType::Position:     return GL_FLOAT;
		case LayoutDataType::UV:	       return GL_FLOAT;
		case LayoutDataType::Color:        return GL_FLOAT;
		case LayoutDataType::Normal:       return GL_FLOAT;
		}

		SPROUT_CORE_ASSERT("Unknown LayoutDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
	{
		SPROUT_CORE_ASSERT(buffer->GetLayout()->GetDesc().GetElements().size(), "Vertex buffer has no layout!");

		glBindVertexArray(RendererID);
		buffer->Bind();

		uint32_t index = 0;
		for (const auto& element : buffer->GetLayout()->GetDesc().GetElements())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetComponentCount(), 
				LayoutDataTypeToOpenGLBaseType(element.LayoutType),
				GL_FALSE, 
				buffer->GetLayout()->GetDesc().GetStride(),
				(const void*) element.Offset);
			index++;
		}

		VBuffer.push_back(buffer);
	}
	
	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<Sprout::IndexBuffer>& buffer)
	{
		glBindVertexArray(RendererID);
		buffer->Bind();
		IBuffer = buffer;
	}
}