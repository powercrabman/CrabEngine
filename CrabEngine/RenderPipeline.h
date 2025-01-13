#pragma once

namespace crab
{
	struct RenderPipeline
	{
		void Clear()
		{
			texture = nullptr;
			shader  = nullptr;

			blendState        = eBlendState::Unknown;
			samplerState      = eSamplerState::Unknown;
			depthStencilState = eDepthStencilState::Unknown;
			rasterizerState   = eRasterizerState::Unknown;
		}

		Ref<ITexture>		texture = nullptr;
		Ref<IShader>		shader  = nullptr;

		eBlendState			blendState        = eBlendState::Unknown;
		eSamplerState		samplerState      = eSamplerState::Unknown;
		eDepthStencilState	depthStencilState = eDepthStencilState::Unknown;
		eRasterizerState	rasterizerState   = eRasterizerState::Unknown;
	};
}


