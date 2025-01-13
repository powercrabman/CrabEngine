#include "CrabEnginePch.h"
#include "IDepthStencil.h"

#include "DX11DepthStencil.h"


namespace crab
{
	Ref<IDepthStencil> IDepthStencil::Create(uint32 in_width, uint32 in_height)
	{
		Ref<IDepthStencil> ds;
		
		switch (Renderer::GetRenderAPIType())
		{

		case eRenderAPI::DirectX11:
			ds = MakeRef<DX11DepthStencil>();
			break;

		default: assert(false);
		}

		ds->_create_(in_width, in_height);
		return ds;
	}
}

