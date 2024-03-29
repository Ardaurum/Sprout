#pragma once

#include "Sprout/Renderer/RendererContext.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX12ContextInternal;

	class DX12Context : public RendererContext
	{
	public:
		DX12Context(Window* windowHandle);

		void Init() override;
		void SwapBuffers() override;
		void WaitForGPU() override;

		void OnResize();

		inline ID3D12Device* GetDevice() { return Device.Get(); }
		//inline ID3D12RenderTargetView* GetRenderTargetView() { return RenderTarget.Get(); }

		static constexpr UINT FRAME_COUNT = 2;

	private:
		DX12ContextInternal* Core;

	public:
		static inline DX12Context& Get() { return *Instance; }

	private:
		static DX12Context* Instance;
		friend class DX12ContextInternal;
	};
}

