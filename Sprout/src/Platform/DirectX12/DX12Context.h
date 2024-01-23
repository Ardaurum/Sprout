#pragma once
#include "Sprout/Renderer/RendererContext.h"

#include <d3d12.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX12Context : public RendererContext
	{
	public:
		DX12Context(Window* windowHandle);

		void Init() override;
		void SwapBuffers() override;

		void OnResize();

		//inline ID3D12Device* GetDevice() { return Device.Get(); }
		//inline ID3D12DeviceContext* GetDeviceContext() { return Context.Get(); }
		//inline ID3D12RenderTargetView* GetRenderTargetView() { return RenderTarget.Get(); }

	private:
		Window* WindowHandle;
		
		/*Microsoft::WRL::ComPtr<ID3D12Device> Device;
		Microsoft::WRL::ComPtr<ID3D12DeviceContext> Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
		Microsoft::WRL::ComPtr<ID3D12RenderTargetView> RenderTarget;
		Microsoft::WRL::ComPtr<ID3D12Texture2D> DepthStencil;
		Microsoft::WRL::ComPtr<ID3D12DepthStencilView> DepthStencilView;*/

#ifdef SPROUT_DEBUG
		Microsoft::WRL::ComPtr<ID3D12Debug> DebugInterface;
#endif

	public:
		static inline DX12Context& Get() { return *Instance; }

	private:
		static DX12Context* Instance;
	};
}

