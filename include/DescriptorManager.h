#pragma once
#if OYABUN_USE_DIRECT3D 
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#endif

namespace Oyabun
{
	class DescriptorManager
	{
	public:
		DescriptorManager();
		virtual ~DescriptorManager();
#if OYABUN_USE_DIRECT3D
		bool CreateRTVHeap(ID3D12Device* pDev, int descriptors);
		bool CreateDSVHeap(ID3D12Device* pDev, int descriptors);
		bool CreateCBVSRVUAVHeap(ID3D12Device* pDev, int descriptors); //For a big game engine, we recommend more than 16384 descriptor slots.

		D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle(int slot) const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const;

		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVCPUHandle(int slot) const;
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVGPUHandle(int slot) const;
#endif
		void DestroyHeaps();
	private:
#if OYABUN_USE_DIRECT3D
		ComPtr<ID3D12DescriptorHeap> m_pRTVHeap;
		ComPtr<ID3D12DescriptorHeap> m_pDSVHeap;
		ComPtr<ID3D12DescriptorHeap> m_pCBVSRVUAVHeap;
		int m_rtvDescriptorSize;
		int m_dsvDescriptorSize;
		int m_srvDescriptorSize;
#endif
	};
}