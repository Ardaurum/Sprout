#include "Sprout.h"
#include "Sprout/Core/EntryPoint.h"

#include <chrono>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

class SandboxLayer : public Sprout::Layer
{
public:
	SandboxLayer(float aspect) : Layer("Sandbox") 
	{
		CamController = std::make_shared<Sprout::CameraController>(Sprout::CameraType::Perspective, aspect);
		CamController->SetPosition(glm::vec3(.0f, .0f, 5.0f));
		
		std::string glslShaderSrc = R"(
			#ifdef __SHADER_TYPE_VERTEX__
			layout(location = 0) in vec3 a_Position;
			layout(location = 2) in vec4 a_Color;

			layout (std140, binding = 0) uniform uPerFrame
			{
				mat4 u_ViewProjection;
				float u_Time;
			};

			layout (std140, binding = 1) uniform uPerObject
			{
				uniform mat4 u_Model;
				uniform vec4 u_Tint;
			};

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
				v_Position = a_Position;
				v_Color = a_Color * u_Tint;
			}
			#endif

			#ifdef __SHADER_TYPE_FRAGMENT__
			layout(location = 0) out vec4 out_color;

			layout (std140, binding = 0) uniform uPerFrame
			{
				mat4 u_ViewProjection;
				float u_Time;
			};

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				out_color = v_Color * (sin(u_Time) + 1.0) / 2.0;
			}
			#endif	
		)";

		std::string hlslShaderSrc = R"(
			cbuffer cbPerFrame : register(b0)
			{
				float4x4 u_ViewProjection;
				float u_Time;
			};

			cbuffer cbPerObject : register(b1)
			{
				float4x4 u_Model;
				float4 u_Tint;
			};

			struct vin {
				float3 Position : POSITION;
				float4 Color : COLOR;
			};

			struct v2p {
				float4 Position : SV_POSITION;
				float4 Color : COLOR;
			};

			v2p vMain(vin v)
			{
				v2p o;
				o.Position = mul(u_ViewProjection, mul(u_Model, float4(v.Position, 1.0)));
				o.Color = v.Color * u_Tint;
				return o;
			}
			
			float4 pMain(v2p p) : SV_TARGET
			{
				return p.Color * (sin(u_Time) + 1.0) / 2.0;
			}
		)";

		//GLSL Shaders
		ShaderLibrary.LoadFromSrc("PulseShader", glslShaderSrc);
		ShaderLibrary.LoadFromPath("assets/shaders/Texture.glsl");

		//HLSL Shaders
		//ShaderLibrary.LoadFromSrc("PulseShader", hlslShaderSrc);
		//ShaderLibrary.LoadFromPath("assets/shaders/Texture.hlsl");

		Texture = Sprout::Texture2D::Create("assets/textures/Checkerboard.png");
		PoppyTexture = Sprout::Texture2D::Create("assets/textures/Poppy.png");

		float vertices[4 * 9] = {
			-0.5f, -0.5f, 0.0f, .0f, .0f, 1.0f, .0f, .0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, .0f, .0f, 1.0f, .0f, 1.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f, .0f, .0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, .0f, 1.0f, 1.0f, .0f, 1.0f, 1.0f
		};
		uint32_t indices[6] = { 2, 1, 0, 3, 2, 0 };

		Sprout::LayoutDataType layoutDesc = Sprout::LayoutDataType::Position | Sprout::LayoutDataType::UV | Sprout::LayoutDataType::Color;

		SquareVA = Sprout::VertexArray::Create();
		std::shared_ptr<Sprout::VertexBuffer> vBuffer = Sprout::VertexBuffer::Create(vertices, sizeof(vertices));
		vBuffer->SetLayout(layoutDesc);
		SquareVA->AddVertexBuffer(vBuffer);
		std::shared_ptr<Sprout::IndexBuffer> iBuffer = Sprout::IndexBuffer::Create(indices, 6);
		SquareVA->SetIndexBuffer(iBuffer);
	}

	void Update(Sprout::Timestep ts) override 
	{
		float dt = ts;
		AccumulatedTime += dt;

		CamController->OnUpdate(ts);

		Sprout::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Sprout::RenderCommand::Clear();

		Sprout::Renderer::BeginScene(CamController->GetCamera());

		/*std::shared_ptr<Sprout::Material> material = std::make_shared<Sprout::Material>(Shader);
		std::shared_ptr<Sprout::MaterialInstance> materialInstance = std::make_shared<Sprout::MaterialInstance>(material);
		materialInstance->Set("u_Tint", glm::vec4(1.0f, .0f, 1.0f, 1.0f));*/

		std::shared_ptr<Sprout::Shader> shader = ShaderLibrary.Get("PulseShader");

		for (int i = 0; i < 5; i++)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(i * 1.5f, .0f, .0f));
			Sprout::Renderer::Submit(shader, SquareVA, transform);
		}
		
		std::shared_ptr<Sprout::Shader> texShader = ShaderLibrary.Get("Texture");

		texShader->Bind();
		Texture->Bind();
		Sprout::Renderer::Submit(texShader, SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, .0f, 2.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		PoppyTexture->Bind();
		Sprout::Renderer::Submit(texShader, SquareVA, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, .0f, 2.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Sprout::Renderer::EndScene();
	}

	void OnEvent(Sprout::Event& ev) override 
	{
		CamController->OnEvent(ev);
	}

	void OnImGuiRender() override
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Camera settings");
		glm::vec3 oldCamPos = CamController->GetPosition();
		float pos[3] = { oldCamPos.x, oldCamPos.y, oldCamPos.z };
		ImGui::DragFloat3("Position", pos, .01f, -10.0f, 10.0f);
		CamController->SetPosition(glm::vec3(pos[0], pos[1], pos[2]));
		glm::vec3 oldCamRot = glm::degrees(CamController->GetRotation());
		float rot[3] = { oldCamRot.x, oldCamRot.y, oldCamRot.z };
		ImGui::DragFloat3("Rotation", rot, .5f, -180.0f, 180.0f);
		CamController->SetRotation(glm::radians(glm::vec3(rot[0], rot[1], rot[2])));
		ImGui::End();
	}

private:
	Sprout::ShaderLibrary ShaderLibrary;

	std::shared_ptr<Sprout::VertexArray> SquareVA;
	std::shared_ptr<Sprout::CameraController> CamController;
	std::shared_ptr<Sprout::Texture2D> Texture, PoppyTexture;

	glm::vec2 MousePos;
	float AccumulatedTime = .0f;
};

class Sandbox : public Sprout::App
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{
	
	}
};

Sprout::App* Sprout::CreateApp()
{
	Sandbox* sandbox = new Sandbox();
	sandbox->PushLayer(new SandboxLayer(sandbox->GetWindow().GetAspect()));
	return sandbox;
}
