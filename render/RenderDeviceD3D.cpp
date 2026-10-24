#include "RenderDevice.h"

#if OYABUN_USE_DIRECT3D

namespace Oyabun
{
	RenderDevice::RenderDevice()
	{

	}

	RenderDevice::~RenderDevice()
	{

	}

	bool RenderDevice::Initialize()
	{
		DWORD dwFlags = 0;
#if _DEBUG
		dwFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		HRESULT hr = CreateDXGIFactory2(dwFlags, IID_PPV_ARGS(&m_pFactory));

		if (FAILED(hr)) return false; //Log error and return false.

		//Note: Write a GPU adapter enumerator helper later
		//that allows us to choose the best GPU by feature classification.
		//Support FL 11_0 as a minimum so even older GPUs can run.
		for (UINT i = 0; m_pFactory->EnumAdapters1(i, &m_pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
			if (SUCCEEDED(D3D12CreateDevice(m_pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
				break;
			}
			m_pAdapter.Reset();
		}

		hr = D3D12CreateDevice(m_pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));

		if (FAILED(hr)) return false; //Log error and return false.

		hr = m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pFence));

		if (FAILED(hr)) return false; //Log error and return false.

		m_fenceValue = 1;
		m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

		if (!m_fenceEvent) return false;

		return true;
	}

	bool RenderDevice::InitCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC cq = {};
		cq.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cq.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		HRESULT hr = m_pDevice->CreateCommandQueue(&cq, IID_PPV_ARGS(&m_pCmdQueue));

		if (FAILED(hr)) return false; //Log error and return false.

		return true;
	}

	bool RenderDevice::InitSwapChain(int width, int height)
	{
		DXGI_SWAP_CHAIN_DESC1 sc = {};
		sc.BufferCount = m_uBufferCount;
		sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sc.Flags = 0;
		sc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sc.Width = width;
		sc.Height = height;
		sc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		sc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		ComPtr<IDXGISwapChain1> p1SwapChain;

		HRESULT hr = m_pFactory->CreateSwapChainForHwnd(m_pDevice.Get(), reinterpret_cast<HWND>(m_hWnd), &sc, nullptr, nullptr, &p1SwapChain);

		if (FAILED(hr)) return false;

		hr = p1SwapChain.As(&m_pSwapChain);

		if (FAILED(hr)) return false;

		return true;
	}

	void RenderDevice::Shutdown()
	{
		WaitForGPU();

		if (m_pSwapChain) m_pSwapChain.Reset();
		if (m_pCmdQueue) m_pCmdQueue.Reset();
		if (m_pDevice) m_pDevice.Reset();
		if (m_pAdapter) m_pAdapter.Reset();
		if (m_pFactory) m_pFactory.Reset();

	}
}

#endif