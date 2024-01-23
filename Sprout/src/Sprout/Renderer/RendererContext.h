#pragma once

namespace Sprout
{
	class Window;

	class RendererContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		static std::shared_ptr<RendererContext> Create(Window* windowHandle);
	};
}
