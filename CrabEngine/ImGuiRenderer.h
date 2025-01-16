#pragma once
#include "IImGuiAPI.h"

namespace crab
{
	class ImGuiRenderer
	{
	public:
		static void Init(const ApplicationSetting& in_setting);
		static void Shutdown();

		static void BeginRender();
		static void EndRender();
		
		static void SetCurrentWindowDrawlistRenderState(
			eBlendState in_blendState, 
			eSamplerState in_samplerState, 
			eDepthStencilState in_depthStencilState, 
			eRasterizerState in_rasterizerState)
		{
			m_api->SetCurrentWindowDrawlistRenderState(in_blendState, in_samplerState, in_depthStencilState, in_rasterizerState);
		}

	private:
		inline static Scope<IImGuiAPI> m_api = nullptr;
	};
}