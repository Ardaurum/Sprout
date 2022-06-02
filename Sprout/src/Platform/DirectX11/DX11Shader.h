#pragma once

#include "Sprout/Renderer/Shader.h"
#include "Sprout/Renderer/InputLayout.h"

#include <d3d11.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX11Shader : public Shader
	{
	public:
		DX11Shader(const std::string& name, const std::string& source);
		~DX11Shader();

		void Bind() const;
		void Unbind() const;

		virtual const std::string& GetName() const override { return Name; }
		const LayoutDataType GetLayoutType() const { return LayoutType; }
		
		const ID3D11VertexShader* GetVertexShader() const { return VertexShader.Get(); }
		const ID3D11PixelShader* GetPixelShader() const { return PixelShader.Get(); }

	public:
		static const DX11Shader* GetCurrent() { return Current; }

	private:
		static const DX11Shader* Current;

	private:
		void Compile(const std::string& shaderSrc);

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> PixelShader;

		std::string Name;
		LayoutDataType LayoutType;
	};
}