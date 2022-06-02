#pragma once

#include "Sprout/Renderer/RendererAPI.h"

namespace Sprout
{
	class OpenGLRendererAPI : public RendererAPI
	{
		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vArray) override;
	};
}