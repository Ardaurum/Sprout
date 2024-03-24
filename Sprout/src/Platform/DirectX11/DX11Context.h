#pragma once
#include "Sprout/Renderer/RendererContext.h"

#include <d3d11.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX11Context : public RendererContext
	{
	public:
		DX11Context(Window* windowHandle);

		void Init() override;
		void SwapBuffers() override;
		void WaitForGPU() override {}

		void OnResize();

		inline ID3D11Device* GetDevice() { return Device.Get(); }
		inline ID3D11DeviceContext* GetDeviceContext() { return Context.Get(); }
		inline ID3D11RenderTargetView* GetRenderTargetView() { return RenderTarget.Get(); }

	private:
		Window* WindowHandle;
		
		Microsoft::WRL::ComPtr<ID3D11Device> Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
		Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencil;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

	public:
		static inline DX11Context& Get() { return *Instance; }

	private:
		static DX11Context* Instance;
	};
}

