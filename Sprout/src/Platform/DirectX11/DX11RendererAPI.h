#pragma once

#include "Sprout/Renderer/RendererAPI.h"

namespace Sprout
{
	class DX11LayoutShaderLibrary;

	class DX11RendererAPI : public RendererAPI
	{
	public:
		DX11RendererAPI();

		virtual void Init() override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vArray) override;

	private:
		FLOAT ClearColor[4];

	// System Singletons
		std::unique_ptr<DX11LayoutShaderLibrary> LayoutShaderLibrary;
	};
}