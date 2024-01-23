#include "spch.h"
#include "DX11Texture.h"

#include "DX11Context.h"

#include <d3d11.h>
#include <stb_image.h>

namespace Sprout
{
	DX11Texture2D::DX11Texture2D(const std::string& path)
		: Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(0);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		SPROUT_CORE_ASSERT_MSG(data, "Failed to load an image!");

		Width = width;
		Height = height;

		D3D11_TEXTURE2D_DESC desc;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.MipLevels = 0;
		desc.ArraySize = 1;
		desc.Width = width;
		desc.Height = height;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		D3D11_SHADER_RESOURCE_VIEW_DESC resViewDesc;
		resViewDesc.Format = desc.Format;
		resViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resViewDesc.Texture2D.MipLevels = -1;
		resViewDesc.Texture2D.MostDetailedMip = 0;

		HRESULT result = DX11Context::Get().GetDevice()->CreateTexture2D(&desc, nullptr, TextureHandle.GetAddressOf());
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create a 2D texture!");

		DX11Context::Get().GetDeviceContext()->UpdateSubresource(TextureHandle.Get(), 0, nullptr, data, (width * 4), 0);

		result = DX11Context::Get().GetDevice()->CreateShaderResourceView(TextureHandle.Get(), &resViewDesc, ResourceHandle.GetAddressOf());
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create a resource view for texture!");

		stbi_image_free(data);
	}

	DX11Texture2D::~DX11Texture2D()
	{
	}

	void DX11Texture2D::Bind(uint32_t slot) const
	{
		DX11Context::Get().GetDeviceContext()->VSSetShaderResources(slot, 1, ResourceHandle.GetAddressOf());
		DX11Context::Get().GetDeviceContext()->PSSetShaderResources(slot, 1, ResourceHandle.GetAddressOf());
	}
}

