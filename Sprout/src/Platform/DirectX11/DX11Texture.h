#pragma once

#include "Sprout/Renderer/Texture.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX11Texture2D : public Texture2D
	{
	public:
		DX11Texture2D(const std::string& path);
		virtual ~DX11Texture2D();

		virtual uint32_t GetWidth() const override { return Width; }
		virtual uint32_t GetHeight() const override { return Height; }

		virtual void Bind(uint32_t slot = 0) const override;

	private:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> TextureHandle;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ResourceHandle;

		std::string Path;
		uint32_t Width;
		uint32_t Height;
	};
}