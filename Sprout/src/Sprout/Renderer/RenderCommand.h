#pragma once

#include "Renderer.h"
#include "RendererAPI.h"

namespace Sprout
{
	class RenderCommand
	{
	public:
		inline static void Init() { API->Init(); }

		inline static void SetClearColor(const glm::vec4& color) { API->SetClearColor(color); }
		inline static void Clear() { API->Clear(); }

		inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vArray) { API->DrawIndexed(vArray); }

	private:
		static void SetupAPI();
		static void Destroy();

		static std::unique_ptr<RendererAPI> API;

		friend class Renderer;
	};
}