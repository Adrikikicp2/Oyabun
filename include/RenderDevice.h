#pragma once

#if OYABUN_USE_DIRECT3D 
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

using namespace Microsoft::WRL;
#endif
#include <cstdint>

namespace Oyabun
{
	class RenderResource; //forward decl
	struct ResourceDesc;

	class RenderDevice
	{
	public:
		RenderDevice();
		virtual ~RenderDevice();
		bool Initialize();
		bool InitCommandQueue();
		bool InitSwapChain(int width, int height);

		void SetBufferCount(int bufferCount) { m_uBufferCount = bufferCount; }

		void Shutdown();

		void WaitForGPU();

		RenderResource* CreateResource(ResourceDesc* pDesc);

#if OYABUN_USE_DIRECT3D
	void FetchWindow(void* window) { m_hWnd = window; }
#endif
	private:
#if OYABUN_USE_DIRECT3D
		ComPtr<IDXGIFactory7> m_pFactory;
		ComPtr<IDXGIAdapter1> m_pAdapter;
		ComPtr<ID3D12Device4> m_pDevice;
		ComPtr<ID3D12CommandAllocator> m_pCmdAllocator;
		ComPtr<ID3D12CommandQueue> m_pCmdQueue;

		ComPtr<ID3D12Fence1> m_pFence;
		UINT m_fenceValue = 0;
		void* m_fenceEvent = nullptr;
		void* m_hWnd = nullptr;

		ComPtr<IDXGISwapChain4> m_pSwapChain = nullptr;
#endif
		uint32_t m_uBufferCount = 2;
		uint32_t m_uCurrentBackBuffer = 0;
	};
}