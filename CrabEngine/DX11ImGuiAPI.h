#pragma once
#include "IImGuiAPI.h"

namespace crab
{
	class DX11ImGuiAPI : public IImGuiAPI
	{
	public:
		virtual void Init() override;

		virtual void Shutdown() override
		{
			ImGui_ImplDX11_Shutdown();
		}

		virtual void RenderBegin() override
		{
			ImGui_ImplDX11_NewFrame();
		}

		virtual void RenderEnd() override
		{
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		}
	};
}


