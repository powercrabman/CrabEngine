#include "CrabEnginePch.h"
#include "IVertexBuffer.h"
#include "DX11VertexBuffer.h"

namespace crab
{
	Ref<IVertexBuffer> IVertexBuffer::_create_concrete_vertex_buffer_(eRenderAPI in_api)
	{
		switch (in_api)
		{
		case eRenderAPI::DirectX11: return MakeRef<DX11VertexBuffer>();
		default: assert(false); 	
		}

		return nullptr;
	}

	Ref<IDynamicVertexBuffer> IDynamicVertexBuffer::_create_concrete_dynamic_vertex_buffer_(eRenderAPI in_api)
	{
		switch (in_api)
		{
		case eRenderAPI::DirectX11: return MakeRef<DX11DynamicVertexBuffer>();
		default: assert(false);
		}

		return nullptr;
	}
}
