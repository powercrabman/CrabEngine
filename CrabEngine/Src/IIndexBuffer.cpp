#include "CrabEnginePch.h"
#include "IIndexBuffer.h"
#include "DX11IndexBuffer.h"

namespace crab
{
	Ref<IIndexBuffer> IIndexBuffer::Create(const std::vector<uint32>& in_indices)
	{
		Ref<IIndexBuffer> buffer;

		switch (Renderer::GetRenderAPIType())
		{
		case eRenderAPI::DirectX11:
			buffer = MakeRef<DX11IndexBuffer>();
			break;

		default: assert(false); break;
		}

		buffer->_create_(in_indices, in_indices.size());
		return buffer;
	}

	Ref<IDynamicIndexBuffer> IDynamicIndexBuffer::Create(const std::vector<uint32>& in_indices, uint32 in_bufferSize)
	{
		Ref<IDynamicIndexBuffer> buffer;

		switch (Renderer::GetRenderAPIType())
		{
		case eRenderAPI::DirectX11:
			buffer = MakeRef<DX11DynamicIndexBuffer>();
			break;

		default: assert(false); break;
		}

		buffer->_create_(in_indices, in_bufferSize);
		return buffer;
	}
}

