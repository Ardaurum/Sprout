#include "spch.h"
#include "DX11VertexArray.h"

namespace Sprout
{
	void DX11VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer)
	{
		SPROUT_CORE_ASSERT(buffer->GetLayout()->GetDesc().GetElements().size(), "Vertex buffer has no layout!");
		VBuffer.push_back(buffer);
	}
	
	void DX11VertexArray::SetIndexBuffer(const std::shared_ptr<Sprout::IndexBuffer>& buffer)
	{
		IBuffer = buffer;
	}
}
