#include "spch.h"
#include "Shader.h"
#include "Renderer.h"

#include "Sprout/FileSystem/FileUtils.h"

#include "Platform/DirectX11/DX11Shader.h"

namespace Sprout
{
	std::shared_ptr<Shader> Shader::CreateFromFile(const std::string& name, const std::string& path)
	{
		switch(Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::Direct3D11:
			return std::make_shared<DX11Shader>(name, FileUtils::ReadFile(path));
		}

		SPROUT_CORE_FATAL("Unknown Renderer API!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::CreateFromFile(const std::string& path)
	{
		std::filesystem::path filepath = path;
		std::string name = filepath.stem().string();
		
		return CreateFromFile(std::move(name), path);
	}

	std::shared_ptr<Shader> Shader::CreateFromSrc(const std::string& name, const std::string& src)
	{
		switch(Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			SPROUT_CORE_FATAL("RendererAPI::None is not supported!");
			return nullptr;
		case RendererAPI::API::Direct3D11:
			return std::make_shared<DX11Shader>(name, src);
		}

		SPROUT_CORE_FATAL("Unknown Renderer API!");
		return nullptr;
	}
	
	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		SPROUT_CORE_ASSERT_MSG(Shaders.find(name) == Shaders.end(), "Shader already exists!");
		Shaders[name] = shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::LoadFromPath(const std::string& name, const std::string& path)
	{
		std::shared_ptr<Shader> shader = Shader::CreateFromFile(name, path);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::LoadFromPath(const std::string& path)
	{
		std::shared_ptr<Shader> shader = Shader::CreateFromFile(path);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::LoadFromSrc(const std::string& name, const std::string& path)
	{
		std::shared_ptr<Shader> shader = Shader::CreateFromSrc(name, path);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		SPROUT_CORE_ASSERT_MSG(Shaders.find(name) != Shaders.end(), "Shader not found!");
		return Shaders[name];
	}
}
