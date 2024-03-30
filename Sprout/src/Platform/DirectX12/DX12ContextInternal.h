#pragma once

#include "Sprout/Core/MacroUtils.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl/client.h>

namespace Sprout
{
	class DX12Context;
	class DescriptorHeapManager;
	class Window;

	SIMPLE_TYPE_WRAPPER(CBVSRVUAVDescriptorSlot, uint32_t)
	SIMPLE_TYPE_WRAPPER(DSVDescriptorSlot, uint32_t)
	SIMPLE_TYPE_WRAPPER(RTVDescriptorSlot, uint32_t)
	SIMPLE_TYPE_WRAPPER(SamplerDescriptorSlot, uint32_t)

	class DX12ContextInternal
	{
	public:
		DX12ContextInternal(Window* windowHandle);
		~DX12ContextInternal();
		SPROUT_NON_COPYABLE(DX12ContextInternal)

		void Init();
		void SwapBuffers();
		void WaitForGPU();

		void OnResize();

		inline ID3D12Device* GetDevice()
		{
			return Device.Get();
		}
		//inline ID3D12RenderTargetView* GetRenderTargetView() { return RenderTarget.Get(); }

		static constexpr UINT FRAME_COUNT = 2;

	private:
		Window* WindowHandle;

		Microsoft::WRL::ComPtr<ID3D12Device> Device = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Device9> DXRDevice = nullptr;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CommandQueue = nullptr;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> SwapChain = nullptr;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> RTVHeap = nullptr;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CBVSRVHeap = nullptr;

		Microsoft::WRL::ComPtr<ID3D12Resource> BackBuffers[FRAME_COUNT] = {};
		RTVDescriptorSlot BackBufferRTVs[FRAME_COUNT] = {};

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAllocators[FRAME_COUNT] = {};

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList = nullptr;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> DXRCommandList = nullptr;

		/*Microsoft::WRL::ComPtr<ID3D12Texture2D> DepthStencil;
		Microsoft::WRL::ComPtr<ID3D12DepthStencilView> DepthStencilView;*/

		uint32_t Width = 0;
		uint32_t Height = 0;

		uint64_t TimestampFrequency = 0;

		UINT FrameIndex = 0;
		HANDLE FenceEvent = 0;
		UINT64 FenceValues[FRAME_COUNT] = {};
		Microsoft::WRL::ComPtr<ID3D12Fence> Fence[FRAME_COUNT] = {};

		UINT RTVDescriptorSize = 0;
		UINT CBVSRVDescriptorSize = 0;

		std::unique_ptr<DescriptorHeapManager> DescriptorHeaps = nullptr;

#ifdef SPROUT_DEBUG
		bool DebugShaders = true;
#endif

		friend class DX12Context;
	};
}