#include "CrabEnginePch.h"
#include "IRenderTarget.h"
#include "DX11RenderTarget.h"

namespace crab
{
	Ref<IRenderTarget> IRenderTarget::Create(const RenderTargetProp& in_prop)
	{
		Ref<IRenderTarget> rendertarget = nullptr;

		switch (Renderer::GetRenderAPIType())
		{
		case eRenderAPI::DirectX11:
			rendertarget = MakeRef<DX11RenderTarget>();
			break;
		default: ASSERT(false, "this api is not available");
		}

		rendertarget->_create_(in_prop);

		return rendertarget;
	}
}
