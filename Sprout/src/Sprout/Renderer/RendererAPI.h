#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Sprout
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1,
			DirectX11 = 2,
			DirectX12 = 3,
			Vulkan = 4
		};
	
	public:
		virtual ~RendererAPI() {}

		virtual void Init() = 0;
		
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vArray) = 0;

		inline static API GetAPI() { return CurrentAPI; }
		
	private:
		static API CurrentAPI;

		friend class Renderer;
	};
}