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
			Direct3D11 = 1,
			Direct3D12 = 2,
			Vulkan = 3
		};
	
	public:
		virtual ~RendererAPI() {}

		virtual void Init() = 0;
		
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vArray) = 0;

		inline static API GetAPI() { return CurrentAPI; }

		inline static const char* ToString(API api)
		{
			switch (api)
			{
			case API::None: return "None";
			case API::Direct3D11: return "Direct3D11";
			case API::Direct3D12: return "Direct3D12";
			case API::Vulkan: return "Vulkan";
			default: return "[Unknown API]";
			}
		}
		
	private:
		static API CurrentAPI;

		friend class Renderer;
	};
}