#include "CrabEnginePch.h"
#include "IConstantBuffer.h"
#include "DX11ConstantBuffer.h"

namespace crab
{
	Ref<IConstantBuffer> IConstantBuffer::_create_concrete_constant_buffer_(eRenderAPI in_api)
	{
		switch (in_api)
		{
		case crab::eRenderAPI::DirectX11:
			return MakeRef<DX11ConstantBuffer>();

		default: 
			assert(false);
			return nullptr;
		}
	}
}
