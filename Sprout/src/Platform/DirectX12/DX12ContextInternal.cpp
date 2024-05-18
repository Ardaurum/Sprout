#include "spch.h"

#include "DX12ContextInternal.h"
#include "Sprout/Core/Assert.h"
#include "Sprout/Core/Window.h"
#include "Sprout/Core/CLISystem.h"
#include "Sprout/Core/Collections.h"

#include <dxgi.h>
#include <dxgi1_3.h>

namespace Sprout
{
	struct DescriptorHeap
	{
		inline D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(uint32_t descriptorSlot) const
		{
			SPROUT_CORE_ASSERT_MSG(descriptorSlot < (Heap != nullptr ? Heap->GetDesc().NumDescriptors : 0), 
				"Trying to get a descriptor that wasn't allocated! Slot: %ul, Number of descriptors: %ul", 
				descriptorSlot, (Heap != nullptr ? Heap->GetDesc().NumDescriptors : 0));

			D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = Heap->GetCPUDescriptorHandleForHeapStart();
			cpuHandle.ptr += descriptorSlot * HandleSize;
			return cpuHandle;
		}

		inline D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle(uint32_t descriptorSlot) const
		{
			SPROUT_CORE_ASSERT_MSG(descriptorSlot < (Heap != nullptr ? Heap->GetDesc().NumDescriptors : 0), 
				"Trying to get a descriptor that wasn't allocated! Slot: %ul, Number of descriptors: %ul",
				descriptorSlot, (Heap != nullptr ? Heap->GetDesc().NumDescriptors : 0));

			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = Heap->GetGPUDescriptorHandleForHeapStart();
			gpuHandle.ptr += descriptorSlot * HandleSize;
			return gpuHandle;
		}

		D3D12_DESCRIPTOR_HEAP_TYPE HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		uint32_t HandleSize = 0;
		ID3D12DescriptorHeap* Heap = nullptr;
		WCHAR DebugName[128] = {};

		//TODO: Currently only Freelist allocator. We could support ring buffer allocator for single frame allocations.
		static constexpr uint32_t INVALID_ALLOCATION = std::numeric_limits<uint32_t>::max();
		Sprout::Freelist Allocator = {};
	};

	class DescriptorHeapManager
	{
	public:
		DescriptorHeapManager()
		{
			CBVSRVUAVDescriptorHeap.HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			wsprintf(CBVSRVUAVDescriptorHeap.DebugName, L"Sprout_CBVSRVUAV_DescriptorHeap");

			DSVDescriptorHeap.HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			wsprintf(DSVDescriptorHeap.DebugName, L"Sprout_DSV_DescriptorHeap");

			RTVDescriptorHeap.HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			wsprintf(RTVDescriptorHeap.DebugName, L"Sprout_RTV_DescriptorHeap");

			SamplerDescriptorHeap.HeapType = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			wsprintf(SamplerDescriptorHeap.DebugName, L"Sprout_Sampler_DescriptorHeap");
		}

		~DescriptorHeapManager()
		{
			// TODO IMPORTANT: Add garbage collector
		}

		SPROUT_NON_COPYABLE(DescriptorHeapManager)

		inline CBVSRVUAVDescriptorSlot AllocateCBVSRVUAVDescriptor(ID3D12Device& device) { return CBVSRVUAVDescriptorSlot(Allocate(device, CBVSRVUAVDescriptorHeap)); }
		inline DSVDescriptorSlot AllocateDSVDescriptor(ID3D12Device& device) { return DSVDescriptorSlot(Allocate(device, DSVDescriptorHeap)); }
		inline RTVDescriptorSlot AllocateRTVDescriptor(ID3D12Device& device) { return RTVDescriptorSlot(Allocate(device, RTVDescriptorHeap)); }
		inline SamplerDescriptorSlot AllocateSamplerDescriptor(ID3D12Device& device) { return SamplerDescriptorSlot(Allocate(device, SamplerDescriptorHeap)); }

		inline D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(CBVSRVUAVDescriptorSlot descriptorSlot) const { return CBVSRVUAVDescriptorHeap.getCPUHandle(descriptorSlot.value); }
		inline D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle(CBVSRVUAVDescriptorSlot descriptorSlot) const { return CBVSRVUAVDescriptorHeap.getGPUHandle(descriptorSlot.value); }
		inline D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(DSVDescriptorSlot descriptorSlot) const { return DSVDescriptorHeap.getCPUHandle(descriptorSlot.value); }
		inline D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle(DSVDescriptorSlot descriptorSlot) const { return DSVDescriptorHeap.getGPUHandle(descriptorSlot.value); }
		inline D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(RTVDescriptorSlot descriptorSlot) const { return RTVDescriptorHeap.getCPUHandle(descriptorSlot.value); }
		inline D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle(RTVDescriptorSlot descriptorSlot) const { return RTVDescriptorHeap.getGPUHandle(descriptorSlot.value); }
		inline D3D12_CPU_DESCRIPTOR_HANDLE getCPUHandle(SamplerDescriptorSlot descriptorSlot) const { return SamplerDescriptorHeap.getCPUHandle(descriptorSlot.value); }
		inline D3D12_GPU_DESCRIPTOR_HANDLE getGPUHandle(SamplerDescriptorSlot descriptorSlot) const { return SamplerDescriptorHeap.getGPUHandle(descriptorSlot.value); }

	private:
		DescriptorHeap CBVSRVUAVDescriptorHeap;
		DescriptorHeap DSVDescriptorHeap;
		DescriptorHeap RTVDescriptorHeap;
		DescriptorHeap SamplerDescriptorHeap;

		uint32_t Allocate(ID3D12Device& device, DescriptorHeap& heap)
		{
			uint32_t const slot = heap.Allocator.AllocateSlot();
			uint32_t const size = (heap.Heap != nullptr ? heap.Heap->GetDesc().NumDescriptors : 0);
			if(heap.Allocator.GetSize() > size)
			{
				ID3D12DescriptorHeap *descriptorHeap = nullptr;
				D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
				descriptorHeapDesc.Type = heap.HeapType;
				descriptorHeapDesc.NumDescriptors = heap.Allocator.GetSize();
				descriptorHeapDesc.NumDescriptors += ((descriptorHeapDesc.NumDescriptors + 2) >> 1);
				device.CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
				if(descriptorHeap == nullptr) 
				{ 
					heap.Allocator.FreeSlot(slot); 
					return heap.Allocator.INVALID_SLOT; 
				};
				if (size > 0)
				{
					device.CopyDescriptorsSimple(size, descriptorHeap->GetCPUDescriptorHandleForHeapStart(),
						heap.Heap->GetCPUDescriptorHandleForHeapStart(), D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
				}
				heap.HandleSize = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

				// TODO IMPORTANT: Add garbage collector
				//collect(heap.DescriptorHeap);

				descriptorHeap->SetName(heap.DebugName);
				heap.Heap = descriptorHeap;
			}
			return slot;
		}
	};

	DX12ContextInternal::DX12ContextInternal(Window* windowHandle) : WindowHandle(windowHandle)
	{

	}

	DX12ContextInternal::~DX12ContextInternal()
	{
	}

	void DX12ContextInternal::Init()
	{
		UINT createDeviceFlags = 0;
		UINT dxgiFactoryFlags = 0;

		HRESULT result;
#ifdef SPROUT_DEBUG
		{
			Microsoft::WRL::ComPtr<ID3D12Debug1> debugInterface;
			result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create D3D12 Debug Interface!");
			debugInterface->EnableDebugLayer();
			debugInterface->SetEnableSynchronizedCommandQueueValidation(true);
		}
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

		Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory = 0;
		result = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create DXGI Factory!");

		// TODO: Currently we just take the first adapter. We should check a number of adapters and choose the best one.
		if (Sprout::CLISystem::Get().IsTrue(SCLI_ID("use_warp")))
		{
			Microsoft::WRL::ComPtr<IDXGIAdapter> warpAdapter;
			result = dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to enable warp!");

			result = D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&Device));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create device!");
		}
		else
		{
			Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = 0;
			result = dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&dxgiAdapter));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to get dxgiAdapter!");

			result = D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&Device));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create device!");
		}

#ifdef SPROUT_DEBUG
		DebugShaders = Sprout::CLISystem::Get().IsTrue(SCLI_ID("debug_shaders"));
#endif
		Device->QueryInterface(IID_PPV_ARGS(&DXRDevice));
		Device->SetName(L"Sprout_Device");

		// TODO: We'll need a couple of command queues. compute, copy, async etc. For now one simple will suffice.
		// Describe and create the command queue.
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		result = Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&CommandQueue));
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create command queue!");
		CommandQueue->GetTimestampFrequency(&TimestampFrequency);
		CommandQueue->SetName(L"Sprout_CommandQueue");

		Width = WindowHandle->GetWidth();
		Height = WindowHandle->GetHeight();

		{
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.BufferCount = FRAME_COUNT;
			swapChainDesc.Width = Width;
			swapChainDesc.Height = Height;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

			Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
			result = dxgiFactory->CreateSwapChainForHwnd(
				CommandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
				static_cast<HWND>(WindowHandle->GetNativeWindowHandle()),
				&swapChainDesc,
				nullptr,
				nullptr,
				&swapChain
			);
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create swap chain!");
			swapChain->QueryInterface(IID_PPV_ARGS(&SwapChain));
		}

		result = dxgiFactory->MakeWindowAssociation(static_cast<HWND>(WindowHandle->GetNativeWindowHandle()), DXGI_MWA_NO_ALT_ENTER);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to initialize swap chain!");
		FrameIndex = SwapChain->GetCurrentBackBufferIndex();

		// Create descriptor heaps manager.
		DescriptorHeaps = std::make_unique<DescriptorHeapManager>();

		// Create command allocators for each frame.
		for (UINT n = 0; n < FRAME_COUNT; n++)
		{
			result = Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocators[n]));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create command allocator!");
			WCHAR buffer[128];
			wsprintf(buffer, L"Sprout_CommandAllocator%u", n);
			CommandAllocators[n]->SetName(buffer);
		}

		result = Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocators->Get(), nullptr, IID_PPV_ARGS(&CommandList));
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create a command list!");

		Device->QueryInterface(IID_PPV_ARGS(&DXRCommandList));
		CommandList->SetName(L"Sprout_CommandList");

		// Create an event handle to use for frame synchronization.
		FenceEvent = CreateEvent(nullptr, false, false, nullptr);
		if (FenceEvent == nullptr)
		{
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(HRESULT_FROM_WIN32(GetLastError())), "Failed to create event handle!");
		}

		for (UINT n = 0; n < FRAME_COUNT; n++)
		{
			result = Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence[n]));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create GPU fence!");
			WCHAR buffer[128];
			wsprintf(buffer, L"Sprout_Fence%u", n);
			Fence[n]->SetName(buffer);
		}

		memset(FenceValues, 0, FRAME_COUNT);

		for (UINT n = 0; n < FRAME_COUNT; n++)
		{
			result = SwapChain->GetBuffer(n, IID_PPV_ARGS(&BackBuffers[n]));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to get back buffer!");
			WCHAR buffer[128];
			wsprintf(buffer, L"Sprout_BackBuffer%u", n);
			BackBuffers[n]->SetName(buffer);

			BackBufferRTVs[n] = DescriptorHeaps->AllocateRTVDescriptor(*Device.Get());
			SPROUT_CORE_ASSERT_MSG(BackBufferRTVs[n].value != DescriptorHeap::INVALID_ALLOCATION, "Failed to allocate RTV descriptor!");
			Device->CreateRenderTargetView(BackBuffers[n].Get(), nullptr, DescriptorHeaps->getCPUHandle(BackBufferRTVs[n]));
		}

		// Wait for the command list to execute before continuing.
		WaitForGPU();
		OnResize();
	}

	void DX12ContextInternal::OnResize()
	{
		SPROUT_CORE_ASSERT_MSG(Device && SwapChain, "Initialize context first!");

		HRESULT result;
		uint32_t width, height;
		width = WindowHandle->GetWidth();
		height = WindowHandle->GetHeight();

		// Determine if the swap buffers and other resources need to be resized or not.
		if (Width != width || Height != height)
		{
			Width = width;
			Height = height;

			// Flush all current GPU commands.
			WaitForGPU();

			// Release the resources holding references to the swap chain (requirement of
			// IDXGISwapChain::ResizeBuffers) and reset the frame fence values to the
			// current fence value.
			for (UINT n = 0; n < FRAME_COUNT; n++)
			{
				BackBuffers[n].Reset();
				FenceValues[n] = FenceValues[FrameIndex];
			}

			// Resize the swap chain to the desired dimensions.
			DXGI_SWAP_CHAIN_DESC desc = {};
			SwapChain->GetDesc(&desc);
			result = SwapChain->ResizeBuffers(FRAME_COUNT, width, height, desc.BufferDesc.Format, desc.Flags);
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Couldn't resize the swapchain buffer!");

			BOOL fullscreenState;
			result = SwapChain->GetFullscreenState(&fullscreenState, nullptr);

			// Reset the frame index to the current back buffer index.
			FrameIndex = SwapChain->GetCurrentBackBufferIndex();
		}
	}

	void DX12ContextInternal::WaitForGPU()
	{
		// Schedule a Signal command in the queue.
		HRESULT result = CommandQueue->Signal(Fence[FrameIndex].Get(), FenceValues[FrameIndex]);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to set fence!");

		// Wait until the fence has been processed.
		Fence[FrameIndex]->SetEventOnCompletion(FenceValues[FrameIndex], FenceEvent);
		WaitForSingleObjectEx(FenceEvent, INFINITE, FALSE);

		// Increment the fence value for the current frame.
		FenceValues[FrameIndex]++;
	}

	void DX12ContextInternal::SwapBuffers()
	{
		SwapChain->Present(1, DXGI_PRESENT_ALLOW_TEARING);

		const UINT64 currentFenceValue = FenceValues[FrameIndex];
		HRESULT result = CommandQueue->Signal(Fence[FrameIndex].Get(), currentFenceValue);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to set fence!");

		FrameIndex = SwapChain->GetCurrentBackBufferIndex();

		if (Fence[FrameIndex]->GetCompletedValue() < FenceValues[FrameIndex])
		{
			result = Fence[FrameIndex]->SetEventOnCompletion(FenceValues[FrameIndex], FenceEvent);
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to set swap completion event");
			WaitForSingleObjectEx(FenceEvent, INFINITE, FALSE);
		}

		FenceValues[FrameIndex] = currentFenceValue + 1;
	}
}