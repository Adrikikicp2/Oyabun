#pragma once

#if OYABUN_USE_DIRECT3D 
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "D3D12MemAlloc.h"
using namespace Microsoft::WRL;
#endif
#include <cstdint>

namespace Oyabun
{

	class DescriptorManager; //forward decl.

	//Add the missing ones (if any) later.
	enum class ResourceTypes : uint8_t
	{
		Buffer,
		Tex1D,
		Tex2D,
		Tex3D,
		TexCube //should this be handled here?
	};

	enum class BufferTypes : uint8_t
	{
		Vertex,
		Index,
		Constant,
		Structured,
		StructuredUav,
		Raw,
		RawUav
	};


	enum class ResourceFormat : uint32_t
	{
		Unknown = 0,
		// 32-bit color
		R32G32B32A32_TYPELESS = 1,
		R32G32B32A32_FLOAT = 2,
		R32G32B32A32_UINT = 3,
		R32G32B32A32_SINT = 4,
		// 16-bit color
		R16G16B16A16_TYPELESS = 9,
		R16G16B16A16_FLOAT = 10,
		R16G16_SNORM = 37,
		R16G16_FLOAT = 34,

		//Special
		R10G10B10A2_TYPELESS = 23,
		R10G10B10A2_UNORM = 24,
		R11G11B10_FLOAT = 26,

		// 8-bit color
		R8G8B8A8_TYPELESS = 27,
		R8G8B8A8_UNORM = 28,
		R8G8B8A8_UNORM_SRGB = 29,
		// 32-bit depth w/ stencil
		R32G8X24_TYPELESS = 19,
		D32_FLOAT_S8X24_UINT = 20,
		R32_FLOAT_X8X24_TYPELESS = 21,
		X32_TYPELESS_G8X24_UINT = 22,
		// 32-bit depth, no stencil
		R32_TYPELESS = 39,
		D32_FLOAT = 40,
		R32_FLOAT = 41,
		// 24-bit depth w/ 8-bit stencil
		R24G8_TYPELESS = 44,
		D24_UNORM_S8_UINT = 45,
		R24_UNORM_X8_TYPELESS = 46,
		X24_TYPELESS_G8_UINT = 47,
		// 16-bit depth
		R16_TYPELESS = 53,
		R16_FLOAT = 54,
		D16_UNORM = 55,

		//Texture Compression
		BC1_TYPELESS = 70,
		BC1_UNORM = 71,
		BC1_SRGB = 72,
		BC2_TYPELESS = 73,
		BC2_UNORM = 74,
		BC2_SRGB = 75,
		BC3_TYPELESS = 76,
		BC3_UNORM = 77,
		BC3_SRGB = 78,
		BC4_TYPELESS = 79,
		BC4_UNORM = 80,
		BC4_SNORM = 81,
		BC5_TYPELESS = 82,
		BC5_UNORM = 83,
		BC5_SNORM = 84,
		BC6H_UF16 = 94,
		BC6H_SF16 = 95,
		BC6H_TYPELESS = 96,
		BC7_UNORM = 97,
		BC7_SRGB = 98,
		BC7_TYPELESS = 99,
		AYUV = 100
	};


	struct ResourceDesc
	{
		ResourceTypes type = ResourceTypes::Buffer;
		ResourceFormat format = ResourceFormat::Unknown;
		int width = 1;
		int height = 1;
		int depth = 1; //If Texture2D, this is ArraySize. D3D12_RESOURCE_DESC uses DepthOrArraySize instead of separate stuff.
		int mipLevels = 1;
		int sampleCount = 1;

		//Buffer-only
		BufferTypes bufKind = BufferTypes::Vertex;
		int stride = 0;
		int elementCount = 0;
	};

#if OYABUN_USE_DIRECT3D
	struct ResourceContext
	{
		ID3D12Device* device = nullptr;
		IDXGIAdapter* adapter = nullptr;
		DescriptorManager* descriptorMgr = nullptr;
		D3D12MA::Allocator* allocator = nullptr;
	};
#endif

	class RenderResource
	{
	public:

		RenderResource();
		virtual ~RenderResource();
#if OYABUN_USE_DIRECT3D
		void FetchContext(ResourceContext* c) { m_pCtx = c; }
#endif
		bool Create(ResourceDesc* d);

		bool Recreate(int newWidth, int newHeight, int newDepth);

		void Shutdown();

	private:
#if OYABUN_USE_DIRECT3D
		ComPtr<ID3D12Resource> m_pResource; //how many resources will we need? 
		ResourceContext* m_pCtx;
		ComPtr<D3D12MA::Allocation> m_pAllocation;

		ResourceDesc* m_desc;
#endif
		bool CreateViews(); 
	};
}