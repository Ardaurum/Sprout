#include "spch.h"
#include "DX11Context.h"
#include "Sprout/Core/Window.h"

#include <dxgi.h>

namespace Sprout
{
	DX11Context* DX11Context::Instance;

	DX11Context::DX11Context(Window* windowHandle)
		: WindowHandle(windowHandle)
	{
		SPROUT_CORE_ASSERT_MSG(!Instance, "DX11 Context already exists!");
		Instance = this;
	}

	void DX11Context::Init()
	{
		UINT createDeviceFlags = 0;

	#ifdef SPROUT_DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
		D3D_FEATURE_LEVEL featureLevel;
		HRESULT result = D3D11CreateDevice(
			NULL, 
			D3D_DRIVER_TYPE_HARDWARE, 
			NULL, 
			createDeviceFlags, 
			NULL, 
			NULL, 
			D3D11_SDK_VERSION, 
			&Device, 
			&featureLevel, 
			&Context);

		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create D3D11 Device!");

		int width, height;
		width = WindowHandle->GetWidth();
		height = WindowHandle->GetHeight();

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = static_cast<HWND>(WindowHandle->GetNativeWindowHandle());
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Flags = 0;

		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice = 0;
		result = Device->QueryInterface(__uuidof(IDXGIDevice), (void**) &dxgiDevice);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to get dxgiDevice!");

		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = 0;
		result = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**) &dxgiAdapter);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to get dxgiAdapter!");

		DXGI_ADAPTER_DESC dxgiAdapterDesc;
		dxgiAdapter->GetDesc(&dxgiAdapterDesc);

		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory = 0;
		result = dxgiAdapter->GetParent(__uuidof(IDXGIFactory),(void**) &dxgiFactory);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to get dxgiFactory!");

		result = dxgiFactory->CreateSwapChain(Device.Get(), &swapChainDesc, &SwapChain);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create SwapChain!");

		SPROUT_CORE_INFO("DirectX Info:");
		SPROUT_CORE_INFO("Vendor: {0}", dxgiAdapterDesc.VendorId);
		SPROUT_CORE_INFO("Version: {0}", featureLevel);
		char description[128];
		sprintf_s(description, "%ws", dxgiAdapterDesc.Description);
		SPROUT_CORE_INFO("Renderer: {0}", description);

		OnResize();
	}

	void DX11Context::OnResize()
	{
		SPROUT_CORE_ASSERT_MSG(Device && Context && SwapChain, "Initialize context first!");

		int width, height;
		width = WindowHandle->GetWidth();
		height = WindowHandle->GetHeight();

		HRESULT result = SwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to resize swap chain buffers!");

		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = 0;
		result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &backBuffer);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to get buffer from swap chain!");

		result = Device->CreateRenderTargetView(backBuffer.Get(), 0, &RenderTarget);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create Render Target View!");

		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		result = Device->CreateTexture2D(&depthStencilDesc, 0, &DepthStencil);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create depth stencil texture!");

		result = Device->CreateDepthStencilView(DepthStencil.Get(), 0, &DepthStencilView);
		SPROUT_CORE_ASSERT_MSG(SUCCEEDED(result), "Failed to create depth stencil view!");

		Context->OMSetRenderTargets(1, RenderTarget.GetAddressOf(), nullptr);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(width);
		viewport.Height = static_cast<float>(height);
		viewport.MinDepth = .0f;
		viewport.MaxDepth = 1.0f;

		Context->RSSetViewports(1, &viewport);
	}

	void DX11Context::SwapBuffers()
	{
		SwapChain->Present(1, 0);
		Context->OMSetRenderTargets(1, RenderTarget.GetAddressOf(), nullptr);
	}
}