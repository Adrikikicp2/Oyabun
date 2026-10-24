#include "RenderDevice.h"
#include "RenderResource.h"
#include <d3dx12.h>

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

	//DXGI_FORMAT will never change, so there is zero need of static
	//asserts. This is why there is no switch statement.
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
	bool RenderResource::Create(ResourceDesc* d)
	{
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
		
		D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON;

		D3D12MA::ALLOCATION_DESC al = {};
		al.HeapType = D3D12_HEAP_TYPE_DEFAULT;
		al.Flags = D3D12MA::ALLOCATION_FLAG_NONE;

		HRESULT hr = m_pCtx->allocator->CreateResource(&al,&rc,initialState,nullptr,&m_pAllocation,IID_PPV_ARGS(&m_pResource));
		
		if (FAILED(hr)) return false;

		m_desc = d;

		return true;
	}

	bool RenderResource::CreateViews()
	{
		

	}

	void RenderResource::Shutdown()
	{
		if(m_pResource) m_pResource.Reset();
		if (m_pAllocation) m_pAllocation.Reset();
	}
}

#endif