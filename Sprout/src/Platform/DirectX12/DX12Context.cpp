#include "spch.h"
#include "DX12Context.h"
#include "Sprout/Core/Window.h"
#include "Sprout/Core/CLISystem.h"

#include <dxgi.h>
#include <dxgi1_3.h>

namespace Sprout
{
	DX12Context* DX12Context::Instance;

	DX12Context::DX12Context(Window* windowHandle)
		: WindowHandle(windowHandle)
	{
		SPROUT_CORE_ASSERT_MSG(!Instance, "DX11 Context already exists!");
		Instance = this;
	}

	void DX12Context::Init()
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
		result = false;// CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
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

		// Create descriptor heaps.
		{
			// Describe and create a render target view (RTV) descriptor heap.
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = 1000;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			result = Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&RTVHeap));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create RTV Heap!");


			// Describe and create a constant buffer view (CBV) and shader resource view (SRV) descriptor heap.
			D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeapDesc = {};
			cbvSrvHeapDesc.NumDescriptors = 1000;
			cbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			cbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			result = Device->CreateDescriptorHeap(&cbvSrvHeapDesc, IID_PPV_ARGS(&CBVSRVHeap));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create CBV SRV Heap!");

			RTVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			CBVSRVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}

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
			SPROUT_CORE_ASSERT_MSG(HRESULT_FROM_WIN32(GetLastError()), "Failed to create event handle!");
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
			Fence[n]->SetName(buffer);
		}

		// Allocate descriptors for back buffer RTV
		for (UINT n = 0; n < FRAME_COUNT; n++)
		{
			result = SwapChain->GetBuffer(n, IID_PPV_ARGS(&BackBuffers[n]));
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to get back buffer!");
		}

		// Wait for the command list to execute before continuing.
		WaitForGPU();
		OnResize();
	}

	void DX12Context::OnResize()
	{
		SPROUT_CORE_ASSERT_MSG(Device && SwapChain, "Initialize context first!");

		uint32_t width, height;
		width = WindowHandle->GetWidth();
		height = WindowHandle->GetHeight();

		// Determine if the swap buffers and other resources need to be resized or not.
		/*if (Width != width || Height != height)
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
				RenderTargets[n].Reset();
				FenceValues[n] = FenceValues[FrameIndex];
			}

			// Resize the swap chain to the desired dimensions.
			DXGI_SWAP_CHAIN_DESC desc = {};
			m_swapChain->GetDesc(&desc);
			ThrowIfFailed(m_swapChain->ResizeBuffers(FrameCount, width, height, desc.BufferDesc.Format, desc.Flags));

			BOOL fullscreenState;
			ThrowIfFailed(m_swapChain->GetFullscreenState(&fullscreenState, nullptr));
			m_windowedMode = !fullscreenState;

			// Reset the frame index to the current back buffer index.
			m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

			// Update the width, height, and aspect ratio member variables.
			UpdateForSizeChange(width, height);

			LoadSizeDependentResources();
		}

		m_windowVisible = !minimized;*/

		/*SPROUT_CORE_ASSERT_MSG(Device && Context && SwapChain, "Initialize context first!");

		int width, height;
		width = WindowHandle->GetWidth();
		height = WindowHandle->GetHeight();

		HRESULT result = SwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to resize swap chain buffers!");

		Microsoft::WRL::ComPtr<ID3D12Texture2D> backBuffer = 0;
		result = SwapChain->GetBuffer(0, __uuidof(ID3D12Texture2D), (void**) &backBuffer);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to get buffer from swap chain!");

		result = Device->CreateRenderTargetView(backBuffer.Get(), 0, &RenderTarget);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create Render Target View!");

		D3D12_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.Usage = D3D12_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D12_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		result = Device->CreateTexture2D(&depthStencilDesc, 0, &DepthStencil);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create depth stencil texture!");

		result = Device->CreateDepthStencilView(DepthStencil.Get(), 0, &DepthStencilView);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create depth stencil view!");

		Context->OMSetRenderTargets(1, RenderTarget.GetAddressOf(), nullptr);

		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = .0f;
		viewport.MaxDepth = 1.0f;

		Context->RSSetViewports(1, &viewport);*/
	}

	void DX12Context::WaitForGPU()
	{
		// Schedule a Signal command in the queue.
		/*HRESULT result = CommandQueue->Signal(Fence.Get(), FenceValues[FrameIndex]);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to set fence!");

		// Wait until the fence has been processed.
		Fence->SetEventOnCompletion(FenceValues[FrameIndex], FenceEvent);
		WaitForSingleObjectEx(FenceEvent, INFINITE, FALSE);

		// Increment the fence value for the current frame.
		FenceValues[FrameIndex]++;*/
	}

	void DX12Context::SwapBuffers()
	{
		/*SwapChain->Present(1, DXGI_PRESENT_ALLOW_TEARING);

		const UINT64 currentFenceValue = FenceValues[FrameIndex];
		HRESULT result = CommandQueue->Signal(Fence.Get(), currentFenceValue);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to set fence!");

		FrameIndex = SwapChain->GetCurrentBackBufferIndex();

		if (Fence->GetCompletedValue() < FenceValues[FrameIndex])
		{
			result = Fence->SetEventOnCompletion(FenceValues[FrameIndex], FenceEvent);
			SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to set swap completion event");
			WaitForSingleObjectEx(FenceEvent, INFINITE, FALSE);
		}

		FenceValues[FrameIndex] = currentFenceValue + 1;*/
	}
}