#include "CrabEnginePch.h"
#include "IRenderFrame.h"
#include "DX11RenderFrame.h"

namespace crab
{
	Ref<IRenderFrame> IRenderFrame::Create(const RenderFrameProp& in_prop)
	{
		Ref<IRenderFrame> renderFrame = nullptr;
		switch (Renderer::GetRenderAPIType())
		{
		case eRenderAPI::DirectX11:
			renderFrame = MakeRef<DX11RenderFrame>();
			break;

		default: 
			assert(false);
		}

		renderFrame->_create_(in_prop);
		return renderFrame;
	}
}
