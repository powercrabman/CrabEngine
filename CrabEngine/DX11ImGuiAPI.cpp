#include "CrabEnginePch.h"
#include "DX11ImGuiAPI.h"

namespace crab
{
	void DX11ImGuiAPI::Init()
	{
		auto* api = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI());

		ImGui_ImplSDL2_InitForD3D(CrabEngine::Get().GetWindow().GetSDLWindow());
		ImGui_ImplDX11_Init(api->GetDevice().Get(), api->GetContext().Get());
	}
}

