#include "spch.h"
#include "DX11Shader.h"

#include "DX11Context.h"

#pragma comment(lib, "dxguid.lib")

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace Sprout
{
	const DX11Shader* DX11Shader::Current = nullptr;

	DX11Shader::DX11Shader(const std::string& name, const std::string& source)
		: Name(name)
	{
		//TODO: Loading precompiled shaders
		Compile(source);
	}

	DX11Shader::~DX11Shader()
	{
	}

	void DX11Shader::Bind() const
	{
		Current = this;
		DX11Context::Get().GetDeviceContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
		DX11Context::Get().GetDeviceContext()->PSSetShader(PixelShader.Get(), nullptr, 0);
	}

	void DX11Shader::Unbind() const
	{
	}

	void DX11Shader::Compile(const std::string& shaderSrc)
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	#ifdef SPROUT_DEBUG
		flags |= D3DCOMPILE_DEBUG;
		flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif

		Microsoft::WRL::ComPtr<ID3DBlob> vData = 0;
		Microsoft::WRL::ComPtr<ID3DBlob> vCompileInfo = 0;
		HRESULT result = D3DCompile(
			shaderSrc.c_str(), 
			strlen(shaderSrc.c_str()), 
			nullptr, 
			nullptr, 
			D3D_COMPILE_STANDARD_FILE_INCLUDE, 
			"vMain", 
			"vs_5_0", 
			0, 
			0, 
			vData.GetAddressOf(), 
			vCompileInfo.GetAddressOf()
		);

		if (FAILED(result))
		{
			SPROUT_CORE_ERROR("Vertex shader compilation failed:\n {0}", (char*) vCompileInfo->GetBufferPointer());
		}

		Microsoft::WRL::ComPtr<ID3DBlob> pData = 0;
		Microsoft::WRL::ComPtr<ID3DBlob> pCompileInfo = 0;
		result = D3DCompile(
			shaderSrc.c_str(), 
			strlen(shaderSrc.c_str()), 
			nullptr, 
			nullptr, 
			D3D_COMPILE_STANDARD_FILE_INCLUDE, 
			"pMain", 
			"ps_5_0", 
			0, 
			0, 
			pData.GetAddressOf(), 
			pCompileInfo.GetAddressOf()
		);

		if (FAILED(result))
		{
			SPROUT_CORE_ERROR("Pixel shader compilation failed:\n {0}", (char*) pCompileInfo->GetBufferPointer());
		}

		result = DX11Context::Get().GetDevice()->CreateVertexShader(vData->GetBufferPointer(), vData->GetBufferSize(), nullptr, VertexShader.GetAddressOf());
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create vertex shader!");

		result = DX11Context::Get().GetDevice()->CreatePixelShader(pData->GetBufferPointer(), pData->GetBufferSize(), nullptr, PixelShader.GetAddressOf());
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create fragment shader!");

		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> vReflection;
		D3DReflect(vData->GetBufferPointer(), vData->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) vReflection.GetAddressOf());
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create vertex shader reflection!");

		D3D11_SHADER_DESC vDesc;
		vReflection->GetDesc(&vDesc);
		LayoutType = LayoutDataType::None;

		for (size_t i = 0; i < vDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			vReflection->GetInputParameterDesc(i, &paramDesc);
			if (strcmp(paramDesc.SemanticName, "POSITION") == 0)
			{
				LayoutType |= LayoutDataType::Position;
				continue;
			}
			if (strcmp(paramDesc.SemanticName, "TEXCOORD") == 0)
			{
				LayoutType |= LayoutDataType::UV;
				continue;
			}
			if (strcmp(paramDesc.SemanticName, "COLOR") == 0)
			{
				LayoutType |= LayoutDataType::Color;
				continue;
			}
			if (strcmp(paramDesc.SemanticName, "NORMAL") == 0)
			{
				LayoutType |= LayoutDataType::Normal;
				continue;
			}
		}
	}
}