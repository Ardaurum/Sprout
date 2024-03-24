#pragma once
#include "Sprout/Renderer/RendererContext.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

namespace Sprout
{
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

	private:
		static constexpr UINT FRAME_COUNT = 2;

		Window* WindowHandle;
		
		Microsoft::WRL::ComPtr<ID3D12Device> Device;
		Microsoft::WRL::ComPtr<ID3D12Device9> DXRDevice;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CBVSRVHeap;

		Microsoft::WRL::ComPtr<ID3D12Resource> BackBuffers[FRAME_COUNT];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocators[FRAME_COUNT];

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DXRCommandList;

		/*Microsoft::WRL::ComPtr<ID3D12Texture2D> DepthStencil;
		Microsoft::WRL::ComPtr<ID3D12DepthStencilView> DepthStencilView;*/

		uint32_t Width;
		uint32_t Height;

		uint64_t TimestampFrequency;

		UINT FrameIndex;
		HANDLE FenceEvent;
		UINT64 FenceValues[FRAME_COUNT];
		Microsoft::WRL::ComPtr<ID3D12Fence> Fence[FRAME_COUNT];

		UINT RTVDescriptorSize;
		UINT CBVSRVDescriptorSize;

#ifdef SPROUT_DEBUG
		bool DebugShaders;
#endif

	public:
		static inline DX12Context& Get() { return *Instance; }

	private:
		static DX12Context* Instance;
	};
}

