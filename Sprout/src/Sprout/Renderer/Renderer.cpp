#include "spch.h"
#include "Renderer.h"
#include "ConstantBuffer.h"

namespace Sprout
{
	struct SceneData
	{
		struct FrameData
		{
			glm::mat4 ViewProjectionMatrix;
			float Time;
		};

		FrameData Frame;
		std::shared_ptr<ConstantBuffer> FrameBuffer;

		struct ObjectData
		{
			glm::mat4 ModelMatrix;
			glm::vec4 Tint;
		};

		ObjectData Object;
		std::shared_ptr<ConstantBuffer> TempObjectBuffer;
	};

	static SceneData Scene;

	void Renderer::PrepareAPI(const RendererAPI::API api)
	{
		RendererAPI::CurrentAPI = api;
		RenderCommand::SetupAPI();
	}

	void Renderer::Init()
	{
		RenderCommand::Init();

		Scene.FrameBuffer = ConstantBuffer::Create(ShaderType::Vertex | ShaderType::Pixel, sizeof(SceneData::FrameData), 0);
		Scene.TempObjectBuffer = ConstantBuffer::Create(ShaderType::Vertex | ShaderType::Pixel, sizeof(SceneData::ObjectData), 1);
	}

	void Renderer::BeginScene(const Camera& camera)
	{
		Scene.Frame.ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		Scene.Frame.Time = 1.0f;
		Scene.FrameBuffer->SetData(&Scene.Frame, sizeof(SceneData::FrameData));
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray> vArray, const glm::mat4& transform)
	{
		shader->Bind();
		Scene.Object.ModelMatrix = transform;
		Scene.Object.Tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		Scene.TempObjectBuffer->SetData(&Scene.Object, sizeof(SceneData::ObjectData));
		vArray->Bind();
		RenderCommand::DrawIndexed(vArray);
	}
}