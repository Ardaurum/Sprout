#include "spch.h"
#include "DX11InputLayout.h"

#include "DX11Context.h"
#include "DX11Shader.h"

namespace Sprout
{
	constexpr static DXGI_FORMAT LayoutDataTypeDX11Format(LayoutDataType type)
	{
		switch(type)
		{
		case LayoutDataType::Position:  return DXGI_FORMAT_R32G32B32_FLOAT;
		case LayoutDataType::UV:        return DXGI_FORMAT_R32G32_FLOAT;
		case LayoutDataType::Color:     return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case LayoutDataType::Normal:    return DXGI_FORMAT_R32G32B32_FLOAT;
		}

		SPROUT_CORE_ERROR("Unknown LayoutDataType!");
		return DXGI_FORMAT_UNKNOWN;
	}

	constexpr static char* LayoutDataTypeString(LayoutDataType type)
	{
		switch(type)
		{
		case LayoutDataType::Position:  return "float3";
		case LayoutDataType::UV:        return "float2";
		case LayoutDataType::Color:     return "float4";
		case LayoutDataType::Normal:    return "float3";
		}

		SPROUT_CORE_ERROR("Unknown LayoutDataType!");
		return 0;
	}

    DX11InputLayout::DX11InputLayout(const LayoutDesc& desc)
		: Desc(desc)
    {
        std::vector<LayoutElement> elements = desc.GetElements();
		
		std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;
		for (size_t i = 0; i < elements.size(); i++)
		{
			LayoutElement& element = elements[i];
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = element.Semantic.c_str();
			elementDesc.SemanticIndex = element.Slot;
			elementDesc.AlignedByteOffset = i == 0 ? 0 : element.Offset;
			elementDesc.Format = LayoutDataTypeDX11Format(element.LayoutType);
			elementDesc.InputSlot = 0;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;
			layoutDesc.emplace_back(elementDesc);
		}

		std::vector<LayoutShaderData> shaderData = DX11LayoutShaderLibrary::Get()->GetVSDataByInputLayout(desc);
		for (size_t i = 0; i < shaderData.size(); i++)
		{
			Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
			HRESULT result = DX11Context::Get().GetDevice()->CreateInputLayout(
				&layoutDesc[0], 
				static_cast<uint32_t>(layoutDesc.size()), 
				shaderData[i].VSData->GetBufferPointer(), 
				shaderData[i].VSData->GetBufferSize(), 
				layout.GetAddressOf()
			);
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Input layout creation failed!");
			Layouts[shaderData[i].Type] = std::move(layout);
		}
    }

	void DX11InputLayout::Bind()
	{
		LayoutDataType shaderLayout = DX11Shader::GetCurrent()->GetLayoutType();
		SPROUT_CORE_ASSERT_MSG(Layouts.find(shaderLayout) != Layouts.end(), "Layout not created for this shader!");
		DX11Context::Get().GetDeviceContext()->IASetInputLayout(Layouts[shaderLayout].Get());
	}

	void DX11InputLayout::Unbind()
	{
	}

	// ======================================= DX11LayoutShaderLibrary ===============================================

	ID3DBlob* DX11LayoutShaderLibrary::GetVSDataByShaderLayout(const LayoutDataType& layoutType)
	{
		LayoutDesc desc = LayoutDesc(layoutType);
		if (Database.find(desc.GetType()) == Database.end())
		{
			constexpr char* shaderTemplate = R"(
				struct LayoutVertex	{ %s };
				float4 vMain(LayoutVertex v) : SV_POSITION { return float4(0, 0, 0, 0); }
			)";

			std::ostringstream shaderLayoutSrc;
			std::vector<LayoutElement> elements = desc.GetElements();
			for (size_t i = 0; i < elements.size(); i++)
			{
				LayoutElement element = elements[i];
				shaderLayoutSrc << LayoutDataTypeString(element.LayoutType) << " element" << i << ":" << element.Semantic << element.Slot << ";\n";
			}

			std::string shaderSrc = shaderTemplate;
			size_t start_pos = shaderSrc.find("%s");
			shaderSrc.replace(start_pos, strlen("%s"), shaderLayoutSrc.str());

			Microsoft::WRL::ComPtr<ID3DBlob> vsData = 0;

			UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
		#ifdef SPROUT_DEBUG
			flags |= D3DCOMPILE_DEBUG;
			flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		#endif

			Microsoft::WRL::ComPtr<ID3DBlob> vsCompileInfo = 0;
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
				vsData.GetAddressOf(), 
				vsCompileInfo.GetAddressOf()
			);

			if (FAILED(result))
			{
				SPROUT_CORE_ERROR("Vertex layout shader compilation failed:\n {0}", (char*) vsCompileInfo->GetBufferPointer());
			}

			Database[desc.GetType()] = vsData;
			return vsData.Get();
		}
		else
		{
			return Database[desc.GetType()].Get();
		}
	}

	std::vector<LayoutShaderData> DX11LayoutShaderLibrary::GetVSDataByInputLayout(const LayoutDesc& layoutType)
	{
		std::vector<LayoutShaderData> data;
		data.emplace_back(LayoutDataType::Position | LayoutDataType::UV, 
			GetVSDataByShaderLayout(LayoutDataType::Position | LayoutDataType::UV));
		data.emplace_back(LayoutDataType::Position | LayoutDataType::Color, 
			GetVSDataByShaderLayout(LayoutDataType::Position | LayoutDataType::Color));
		data.emplace_back(LayoutDataType::Position | LayoutDataType::UV | LayoutDataType::Color, 
			GetVSDataByShaderLayout(LayoutDataType::Position | LayoutDataType::UV | LayoutDataType::Color));
		return data;
	}
}
