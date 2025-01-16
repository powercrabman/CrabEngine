#pragma once

namespace crab
{
	struct RenderPipeline
	{
		void Clear()
		{
			texture = nullptr;
			shader  = nullptr;

			blendState        = eBlendState::None;
			samplerState      = eSamplerState::None;
			depthStencilState = eDepthStencilState::None;
			rasterizerState   = eRasterizerState::None;
		}

		Ref<ITexture>		texture = nullptr;
		Ref<IShader>		shader  = nullptr;

		eBlendState			blendState        = eBlendState::None;
		eSamplerState		samplerState      = eSamplerState::None;
		eDepthStencilState	depthStencilState = eDepthStencilState::None;
		eRasterizerState	rasterizerState   = eRasterizerState::None;
	};
}


