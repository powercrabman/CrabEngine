#include "CrabEnginePch.h"
#include "IUnorderedAccessView.h"

#include "DX11UnorderedAccessView.h"

namespace crab
{
	Ref<IUnorderedAccessView> IUnorderedAccessView::Create(const UnorderedAccessViewProp& in_prop)
	{
		Ref<IUnorderedAccessView> uav;
		switch (Renderer::GetRenderAPIType())
		{
		case eRenderAPI::DirectX11:
			uav = MakeRef<DX11UnorderedAccessView>();
			break;

		default: assert(false);
		}

		uav->_create_(in_prop);
		return uav;
	}
}
