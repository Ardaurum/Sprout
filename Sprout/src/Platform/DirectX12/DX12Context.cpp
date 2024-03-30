#include "spch.h"
#include "DX12Context.h"
#include "DX12ContextInternal.h"

namespace Sprout
{
	DX12Context* DX12Context::Instance;

	DX12Context::DX12Context(Window* windowHandle)
	{
		SPROUT_CORE_ASSERT_MSG(!Instance, "DX11 Context already exists!");
		Core = new DX12ContextInternal(windowHandle);
		Instance = this;
	}

	DX12Context::~DX12Context()
	{
		delete Core;
	}

	void DX12Context::Init()
	{
		Core->Init();
	}

	void DX12Context::OnResize()
	{
		Core->OnResize();
	}

	void DX12Context::WaitForGPU()
	{
		Core->WaitForGPU();
	}

	void DX12Context::SwapBuffers()
	{
		Core->SwapBuffers();
	}
}