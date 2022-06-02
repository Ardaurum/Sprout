#pragma once

#include "Shader.h"

namespace Sprout
{
	class ConstantBuffer
	{
	public:
		virtual ~ConstantBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;

		static std::shared_ptr<ConstantBuffer> Create(ShaderType applyTo, uint32_t size, uint32_t binding);
	};
}