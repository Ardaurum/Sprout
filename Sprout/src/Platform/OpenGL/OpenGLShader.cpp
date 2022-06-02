#include "spch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace Sprout
{
	namespace Utils
	{
		static const GLchar* ShaderTypeToDefine(GLenum type)
		{
			switch(type)
			{
			case (GL_VERTEX_SHADER):
				return "#define __SHADER_TYPE_VERTEX__\n\0";
			case (GL_FRAGMENT_SHADER):
				return "#define __SHADER_TYPE_FRAGMENT__\n\0";
			}

			SPROUT_CORE_ASSERT(false, "Unsupported shader type provided!");
			return "";
		}

		static const GLenum ShaderTypeFromString(const std::string& text)
		{
			if (text == "VERTEX__")
			{
				return GL_VERTEX_SHADER;
			}
			if (text == "FRAGMENT__")
			{
				return GL_FRAGMENT_SHADER;
			}

			SPROUT_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& source)
		: Name(name), RendererID(0)
	{
		std::vector<GLenum> shaderTypes = PreProcess(source);
		Compile(shaderTypes, source);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	std::vector<GLenum> OpenGLShader::PreProcess(const std::string& source)
	{
		std::vector<GLenum> shaderTypes;
		const char* typeToken = "__SHADER_TYPE_";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			SPROUT_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength;
			std::string type = source.substr(begin, eol - begin);
			shaderTypes.push_back(Utils::ShaderTypeFromString(type));
			pos = source.find(typeToken, eol);
		}

		return shaderTypes;
	}

	void OpenGLShader::Compile(const std::vector<GLenum>& shaderTypes, const std::string& shaderSrc)
	{
		GLuint program = glCreateProgram();
		SPROUT_CORE_ASSERT(shaderTypes.size() <= 3, "Only 3 shader types max in one program are supported!");
		std::array<GLenum, 3> glShaderIDs;
		int glShaderIndex = 0;

		for (auto& shaderType : shaderTypes)
		{
			GLuint shader = glCreateShader(shaderType);

			const GLchar* source[] = { 
				"#version 450 core\n\0", 
				Utils::ShaderTypeToDefine(shaderType), 
				(const GLchar*)shaderSrc.c_str() 
			};

			glShaderSource(shader, sizeof(source) / sizeof(*source), source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if(isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
		
				glDeleteShader(shader);

				SPROUT_CORE_ERROR("{0}", infoLog.data());
				SPROUT_CORE_ASSERT(false, "Shader compilation failed!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIndex] = shader;
			glShaderIndex++;
		}

		RendererID = program;

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
	
			glDeleteProgram(program);

			for (GLenum shaderID : glShaderIDs)
			{
				glDeleteShader(shaderID);
			}

			SPROUT_CORE_ERROR("{0}", infoLog.data());
			SPROUT_CORE_ASSERT(false, "Shader link failed!");
			return;
		}

		for (GLenum shaderID : glShaderIDs)
		{
			glDetachShader(program, shaderID);
		}
	}
}