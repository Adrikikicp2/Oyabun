#include "RenderDevice.h"
#include "RenderResource.h"

#if OYABUN_USE_DIRECT3D

namespace Oyabun
{

	D3D12_RESOURCE_DIMENSION GetD3D12Dimension(const ResourceTypes& type)
	{
		switch (type)
		{
		case ResourceTypes::Buffer:
		{
			return D3D12_RESOURCE_DIMENSION_BUFFER;
		}
		case ResourceTypes::Tex1D:
		{
			return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		}
		case ResourceTypes::Tex2D:
		{
			return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		}
		case ResourceTypes::Tex3D:
		{
			return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
		}
		default: return D3D12_RESOURCE_DIMENSION_UNKNOWN;
		}
	}

	DXGI_FORMAT ToDXGI(const ResourceFormat& fmt)
	{
		//hmmm...
		return static_cast<DXGI_FORMAT>(fmt);
	}
	RenderResource::RenderResource()
	{

	}

	RenderResource::~RenderResource()
	{

	}

	bool RenderResource::Initialize()
	{
		if (!m_device) return false;
		if (!m_adapter) return false;
		//if (!m_cmdList) return false;

		D3D12MA::ALLOCATOR_DESC ac = {};
		ac.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
		ac.pAdapter = m_adapter;
		ac.pDevice = m_device;
		HRESULT hr = D3D12MA::CreateAllocator(&ac, &m_pAllocator);

		if (FAILED(hr)) return false;

		return true;
	}

	bool RenderResource::Create(ResourceDesc* d)
	{
		m_desc = d;

		D3D12_RESOURCE_DESC rc = {};
		rc.Dimension = GetD3D12Dimension(d->type);

		rc.Format = ToDXGI(d->format);
		rc.MipLevels = d->mipLevels;
		rc.DepthOrArraySize = d->depth;
		rc.SampleDesc.Count = d->sampleCount;
		rc.Width = d->width;
		rc.Height = d->height;
		rc.Flags = D3D12_RESOURCE_FLAG_NONE;
		if (d->format == ResourceFormat::D16_UNORM || d->format == ResourceFormat::D24_UNORM_S8_UINT || d->format == ResourceFormat::D32_FLOAT || d->format == ResourceFormat::D32_FLOAT_S8X24_UINT)
		{
			rc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}
		
		D3D12MA::ALLOCATION_DESC al = {};
		al.HeapType = D3D12_HEAP_TYPE_DEFAULT;
		al.Flags = D3D12MA::ALLOCATION_FLAG_NONE;
		HRESULT hr = m_pAllocator->CreateResource(&al,&rc,D3D12_RESOURCE_STATE_COPY_DEST,nullptr,&m_pAllocation,IID_PPV_ARGS(&m_pResource));
		
		if (FAILED(hr)) return false;

		return true;
	}
}

#endif