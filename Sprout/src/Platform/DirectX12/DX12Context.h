#pragma once

#include "Sprout/Renderer/RendererContext.h"

namespace Sprout
{
	class DX12ContextInternal;

	class DX12Context : public RendererContext
	{
	public:
		DX12Context(Window* windowHandle);
		~DX12Context();

		void Init() override;
		void SwapBuffers() override;
		void WaitForGPU() override;

		void OnResize();

		//inline ID3D12Device* GetDevice() { return Device.Get(); }
		//inline ID3D12RenderTargetView* GetRenderTargetView() { return RenderTarget.Get(); }

		static inline DX12Context& Get() { return *Instance; }

		static constexpr UINT FRAME_COUNT = 2;

	private:
		DX12ContextInternal* Core;

		static DX12Context* Instance;
	};
}

