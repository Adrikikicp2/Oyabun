#include "DescriptorManager.h"
#include <d3dx12.h>

#if OYABUN_USE_DIRECT3D
namespace Oyabun
{
	DescriptorManager::DescriptorManager()
	{

	}

	DescriptorManager::~DescriptorManager()
	{

	}

	bool DescriptorManager::CreateRTVHeap(ID3D12Device* pDev, int descriptors)
	{
		D3D12_DESCRIPTOR_HEAP_DESC dh = {};
		dh.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		dh.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dh.NumDescriptors = descriptors;

		HRESULT hr = pDev->CreateDescriptorHeap(&dh, IID_PPV_ARGS(&m_pRTVHeap));

		if (FAILED(hr)) return false;

		m_rtvDescriptorSize = pDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		return true;
	}

	bool DescriptorManager::CreateDSVHeap(ID3D12Device* pDev, int descriptors)
	{
		D3D12_DESCRIPTOR_HEAP_DESC dh = {};
		dh.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dh.NumDescriptors = descriptors;
		dh.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		HRESULT hr = pDev->CreateDescriptorHeap(&dh, IID_PPV_ARGS(&m_pRTVHeap));

		if (FAILED(hr)) return false;

		m_dsvDescriptorSize = pDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

		return true;
	}

	bool DescriptorManager::CreateCBVSRVUAVHeap(ID3D12Device* pDev, int descriptors)
	{
		D3D12_DESCRIPTOR_HEAP_DESC dh = {};
		dh.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		dh.NumDescriptors = descriptors;
		dh.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		HRESULT hr = pDev->CreateDescriptorHeap(&dh, IID_PPV_ARGS(&m_pRTVHeap));

		if (FAILED(hr)) return false;

		m_srvDescriptorSize = pDev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		return true;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorManager::GetRTVHandle(int slot) const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRTVHeap->GetCPUDescriptorHandleForHeapStart(), slot, m_rtvDescriptorSize);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorManager::GetDSVHandle() const
	{
		return m_pDSVHeap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DescriptorManager::GetSRVCPUHandle(int slot) const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			m_pCBVSRVUAVHeap->GetCPUDescriptorHandleForHeapStart(),
			slot,
			m_srvDescriptorSize
		);
	}

	D3D12_GPU_DESCRIPTOR_HANDLE DescriptorManager::GetSRVGPUHandle(int slot) const
	{
		return CD3DX12_GPU_DESCRIPTOR_HANDLE(
			m_pCBVSRVUAVHeap->GetGPUDescriptorHandleForHeapStart(),
			slot,
			m_srvDescriptorSize
		);
	}
}



#endif