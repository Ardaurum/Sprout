#pragma once

#include "Sprout/Renderer/Texture.h"

namespace Sprout
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return Width; }
		virtual uint32_t GetHeight() const override { return Height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		std::string Path;
		uint32_t Width;
		uint32_t Height;
		uint32_t RendererID;
	};
}