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

	struct ResourceDesc
	{
		ResourceTypes type = ResourceTypes::Buffer;
#if OYABUN_USE_DIRECT3D
		int format = DXGI_FORMAT_UNKNOWN;
#else
		int format = 0;
#endif
		int width = 1;
		int height = 1;
		int depth = 1;
		int arraySize = 1;
		int mipLevels = 1;
		int sampleCount = 1;

		//Buffer-only
		BufferTypes bufKind = BufferTypes::Vertex;
		int stride = 0;
		int elementCount = 0;
	};

	class RenderResource
	{
	public:
		bool Initialize();

		void Shutdown();
	};
}