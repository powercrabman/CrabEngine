#include "CrabEnginePch.h"
#include "IIndexBuffer.h"
#include "DX11IndexBuffer.h"

namespace crab
{
	Ref<IIndexBuffer> IIndexBuffer::Create(const std::initializer_list<uint32>& in_indices)
	{
		Ref<IIndexBuffer> buffer;

		switch (Renderer::GetRednerAPIType())
		{
		case eRenderAPI::DirectX11:
			buffer = MakeRef<DX11IndexBuffer>();
			break;

		default: assert(false); break;
		}

		buffer->_create_(in_indices);
		return buffer;
	}
}

