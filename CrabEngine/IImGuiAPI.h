#pragma once

namespace crab
{
	class IImGuiAPI
	{
	public:
		IImGuiAPI() = default;
		virtual ~IImGuiAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void RenderBegin() = 0;
		virtual void RenderEnd() = 0;
		virtual void SetCurrentWindowDrawlistRenderState(eBlendState in_blendState, eSamplerState in_samplerState, eDepthStencilState in_depthStencilState, eRasterizerState in_rasterizerState) = 0;
	};
}
