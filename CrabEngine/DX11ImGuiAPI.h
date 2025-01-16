#pragma once
#include "IImGuiAPI.h"
#include <ImGui/imgui/backends/imgui_impl_dx11.h>

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

		void SetCurrentWindowDrawlistRenderState(eBlendState in_blendState, eSamplerState in_samplerState, eDepthStencilState in_depthStencilState, eRasterizerState in_rasterizerState) override;
	};
}


