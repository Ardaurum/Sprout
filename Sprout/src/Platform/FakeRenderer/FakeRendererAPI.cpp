#include "spch.h"
#include "FakeRendererAPI.h"

namespace Sprout
{
	constexpr char* ErrorMsg = "RendererAPI hasn't been setup! Setup currently used API using RendererAPI::SetAPI(API).\0";

	void FakeRendererAPI::Init()
	{
		SPROUT_CORE_FATAL(ErrorMsg);
	}

	void FakeRendererAPI::SetClearColor(const glm::vec4& color)
	{
		SPROUT_CORE_FATAL(ErrorMsg);
	}

	void FakeRendererAPI::Clear()
	{
		SPROUT_CORE_FATAL(ErrorMsg);
	}

	void FakeRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vArray)
	{
		SPROUT_CORE_FATAL(ErrorMsg);
	}
}