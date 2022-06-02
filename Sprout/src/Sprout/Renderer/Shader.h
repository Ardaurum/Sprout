#pragma once

#include "Sprout/Core/EnumUtils.h"

#include <glm/glm.hpp>
#include <unordered_map>

namespace Sprout
{
	enum class ShaderType
	{
		None = 0,
		Vertex = 1 << 0,
		Pixel = 1 << 1,
		Geometry = 1 << 2
	};

	MAKE_ENUM_FLAG(ShaderType)

	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static std::shared_ptr<Shader> CreateFromSrc(const std::string& name, const std::string& source);
		static std::shared_ptr<Shader> CreateFromFile(const std::string& name, const std::string& path);
		static std::shared_ptr<Shader> CreateFromFile(const std::string& path);
	};

	class ShaderLibrary
	{
	public:
		void Add(const std::shared_ptr<Shader>& shader);

		std::shared_ptr<Shader> LoadFromPath(const std::string& name, const std::string& path);
		std::shared_ptr<Shader> LoadFromPath(const std::string& path);
		std::shared_ptr<Shader> LoadFromSrc(const std::string& name, const std::string& src);

		std::shared_ptr<Shader> Get(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> Shaders;
	};
}