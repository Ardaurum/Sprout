#pragma once

#include "RendererAPI.h"
#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Sprout 
{
	class Renderer
	{
	public:
		static void PrepareAPI(const RendererAPI::API api);
		static void Destroy();

		static void Init();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> vArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}