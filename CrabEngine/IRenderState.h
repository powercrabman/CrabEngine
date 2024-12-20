#pragma once

namespace crab
{
	class IRenderState
	{
	public:
		IRenderState() = default;
		virtual ~IRenderState() = default;

		virtual void BindBlendState(eBlendState in_state)                     = 0;
		virtual void BindDepthStencilState(eDepthStencilState in_state)       = 0;
		virtual void BindRasterizerState(eRasterizerState in_state)           = 0;
		virtual void BindSamplerState(eSamplerState in_state, uint32 in_slot) = 0;
	};
}
