#pragma once

#include "Sprout\Core\EnumUtils.h"

namespace Sprout
{
	enum class LayoutDataType : uint8_t
	{
		None = 0,
		Position = 1 << 0,
		UV = 1 << 1,
		Color = 1 << 2,
		Normal = 1 << 3
	};

	MAKE_ENUM_FLAG(LayoutDataType)

	struct LayoutElement
	{
		LayoutDataType LayoutType;
		std::string Semantic;
		uint32_t Slot;
		uint32_t Offset;
		uint32_t Size;

		LayoutElement() = default;

		LayoutElement(LayoutDataType layoutType, uint32_t size, const std::string& semantic, uint32_t slot)
			: LayoutType(layoutType), Size(size), Semantic(semantic), Slot(slot), Offset(0)
		{ }

		constexpr uint32_t GetComponentCount() const
		{
			switch (LayoutType)
			{
			case LayoutDataType::Position:     return 3;
			case LayoutDataType::UV:           return 2;
			case LayoutDataType::Color:        return 4;
			case LayoutDataType::Normal:       return 3;
			}

			SPROUT_CORE_ASSERT("Unknown LayoutDataType!");
			return 0;
		}
	};

	class LayoutDesc
	{
	public:
		LayoutDesc(LayoutDataType layoutType)
		{
			Type = layoutType;
			if (Any(layoutType & LayoutDataType::Position))
			{
				Elements.push_back(LayoutElement(LayoutDataType::Position, 3 * 4, "POSITION", 0));
			}
			if (Any(layoutType & LayoutDataType::UV))
			{
				Elements.push_back(LayoutElement(LayoutDataType::UV, 2 * 4, "TEXCOORD", 0));
			}
			if (Any(layoutType & LayoutDataType::Color))
			{
				Elements.push_back(LayoutElement(LayoutDataType::Color, 4 * 4, "COLOR", 0));
			}
			if (Any(layoutType & LayoutDataType::Normal))
			{
				Elements.push_back(LayoutElement(LayoutDataType::Normal, 3 * 4, "NORMAL", 0));
			}
			CalculateOffsetsAndStride();
		}

		std::vector<LayoutElement>::iterator begin() { return Elements.begin(); }
		std::vector<LayoutElement>::iterator end() { return Elements.end(); }
		std::vector<LayoutElement>::reverse_iterator rbegin() { return Elements.rbegin(); }
		std::vector<LayoutElement>::reverse_iterator rend() { return Elements.rend(); }

		std::vector<LayoutElement>::const_iterator begin() const { return Elements.begin(); }
		std::vector<LayoutElement>::const_iterator end() const { return Elements.end(); }
		std::vector<LayoutElement>::const_reverse_iterator rbegin() const { return Elements.rbegin(); }
		std::vector<LayoutElement>::const_reverse_iterator rend() const { return Elements.rend(); }

		inline const std::vector<LayoutElement>& GetElements() const { return Elements; }
		inline const uint32_t GetStride() const { return Stride; }
		inline const LayoutDataType GetType() const { return Type; }

	private:
		std::vector<LayoutElement> Elements;
		uint32_t Stride = 0;
		LayoutDataType Type;

		void CalculateOffsetsAndStride()
		{
			Stride = 0;
			for (auto& e : Elements)
			{
				e.Offset = Stride;
				Stride += e.Size;
			}
		}
	};

	class InputLayout
	{
	public:
		virtual ~InputLayout() {}

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const LayoutDesc& GetDesc() const = 0;
	};
}