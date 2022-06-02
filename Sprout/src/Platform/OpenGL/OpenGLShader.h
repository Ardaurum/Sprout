#pragma once

#include "Sprout/Renderer/Shader.h"

#include <glad/glad.h>

namespace Sprout
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& name, const std::string& source);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		virtual const std::string& GetName() const override { return Name; }

	private:
		std::vector<GLenum> PreProcess(const std::string& source);
		void Compile(const std::vector<GLenum>& shaderTypes, const std::string& shaderSrcs);

	private:
		uint32_t RendererID;
		std::string Name;
	};
}