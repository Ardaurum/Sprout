#pragma once

#include "Sprout/Renderer/InputLayout.h"
#include "Sprout/Renderer/Shader.h"

#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX11InputLayout : public InputLayout
	{
	public:
		DX11InputLayout(const LayoutDesc& layout);

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const LayoutDesc& GetDesc() const override { return Desc; }

	private:
		LayoutDesc Desc;
		
		std::unordered_map<LayoutDataType, Microsoft::WRL::ComPtr<ID3D11InputLayout>> Layouts;
	};

	struct LayoutShaderData
	{
		LayoutShaderData(LayoutDataType type, ID3DBlob* vsData)
			: Type(type), VSData(vsData)
		{}

		LayoutDataType Type;
		ID3DBlob* VSData;
	};

	class DX11LayoutShaderLibrary
	{
	public:
		ID3DBlob* GetVSDataByShaderLayout(const LayoutDataType& layoutType);
		std::vector<LayoutShaderData> GetVSDataByInputLayout(const LayoutDesc& layoutType);
		
	private:
		std::unordered_map<LayoutDataType, Microsoft::WRL::ComPtr<ID3DBlob>> Database;

	public:
		static DX11LayoutShaderLibrary* Get() { 
			static DX11LayoutShaderLibrary Instance;
			return &Instance; 
		}
	};
}
