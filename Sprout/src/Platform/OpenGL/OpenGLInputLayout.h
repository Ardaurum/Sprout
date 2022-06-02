#pragma once

#include "Sprout/Renderer/InputLayout.h"

namespace Sprout
{
	class OpenGLInputLayout : public InputLayout
	{
	public:
		OpenGLInputLayout(LayoutDesc desc): Desc(desc) {}

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const LayoutDesc& GetDesc() const override { return Desc; }

	private:
		LayoutDesc Desc;
	};
}
