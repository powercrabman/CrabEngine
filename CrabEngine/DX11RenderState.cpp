#include "CrabEnginePch.h"
#include "DX11RenderState.h"
#include "DX11RenderTarget.h"

namespace crab
{
	DX11RenderState::DX11RenderState()
	{
		auto device = static_cast<DX11RenderAPI*>(Renderer::GetRenderAPI())->GetDevice();

		// BlendState
		for (int i = 0; i < (uint32)eBlendState::Count; ++i)
		{
			D3D11_BLEND_DESC desc = {};
			desc.AlphaToCoverageEnable = FALSE;
			desc.IndependentBlendEnable = FALSE;

			desc.RenderTarget[0].BlendEnable           = FALSE;
			desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlend             = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			switch ((eBlendState)i)
			{
			case crab::eBlendState::Opaque:
				desc.RenderTarget[0].BlendEnable = FALSE;
				break;

			case crab::eBlendState::AlphaBlend:
				desc.RenderTarget[0].BlendEnable    = TRUE;
				desc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
				break;

			case crab::eBlendState::Additive:
				desc.RenderTarget[0].BlendEnable    = TRUE;
				desc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend      = D3D11_BLEND_ONE;
				desc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
				break;

			case crab::eBlendState::NonPremultiplied:
				desc.RenderTarget[0].BlendEnable    = TRUE;
				desc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
				desc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_SRC_ALPHA;
				desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
				desc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
				break;

			default:
				assert(false);
				break;
			}

			DX_ASSERT(device->CreateBlendState(&desc, m_blendStates[i].GetAddressOf()), "blendstate create fail.");
		}


		// DepthStencilState
		for (int i = 0; i < (uint32)eDepthStencilState::Count; ++i)
		{
			D3D11_DEPTH_STENCIL_DESC desc = {};

			switch ((eDepthStencilState)i)
			{
			case crab::eDepthStencilState::NoDepthTest:
				desc.DepthEnable    = FALSE;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc      = D3D11_COMPARISON_ALWAYS;
				desc.StencilEnable  = FALSE;
				break;

			case crab::eDepthStencilState::DepthTest:
				desc.DepthEnable    = TRUE;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				desc.DepthFunc      = D3D11_COMPARISON_LESS; 
				desc.StencilEnable  = FALSE;
				break;

			case crab::eDepthStencilState::DepthOnlyRead:
				desc.DepthEnable    = TRUE;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc      = D3D11_COMPARISON_LESS;
				desc.StencilEnable  = FALSE; 
				break;

			case crab::eDepthStencilState::DepthReverseZ:
				desc.DepthEnable    = TRUE;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
				desc.DepthFunc      = D3D11_COMPARISON_GREATER;
				desc.StencilEnable  = FALSE; 
				break;

			case crab::eDepthStencilState::DepthOnlyReadReverseZ:
				desc.DepthEnable    = TRUE;
				desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; 
				desc.DepthFunc      = D3D11_COMPARISON_GREATER; 
				desc.StencilEnable  = FALSE;
				break;

			default:
				assert(false);
				break;
			}

			DX_ASSERT(device->CreateDepthStencilState(&desc, m_depthStencilStates[i].GetAddressOf()), "DepthStencilState creation failed.");
		}

		// RasterizerState
		for (int i = 0; i < (uint32)eRasterizerState::Count; ++i)
		{
			D3D11_RASTERIZER_DESC desc = {};
			desc.FillMode              = D3D11_FILL_SOLID;
			desc.CullMode              = D3D11_CULL_BACK;
			desc.FrontCounterClockwise = FALSE;
			desc.DepthClipEnable       = TRUE;
			desc.ScissorEnable         = FALSE;
			desc.MultisampleEnable     = TRUE;
			desc.AntialiasedLineEnable = FALSE;

			switch ((eRasterizerState)i)
			{
			case crab::eRasterizerState::NoCull:
				desc.CullMode = D3D11_CULL_NONE;
				break;

			case crab::eRasterizerState::CullClockwise:
				desc.CullMode              = D3D11_CULL_FRONT;
				desc.FrontCounterClockwise = FALSE;
				break;

			case crab::eRasterizerState::CullCounterClockwise:
				desc.CullMode              = D3D11_CULL_BACK;
				desc.FrontCounterClockwise = TRUE;
				break;

			case crab::eRasterizerState::Wireframe:
				desc.FillMode = D3D11_FILL_WIREFRAME;
				desc.CullMode = D3D11_CULL_NONE;
				break;

			default:
				assert(false && "Invalid Rasterizer State");
				break;
			}

			DX_ASSERT(device->CreateRasterizerState(&desc, m_rasterizerStates[i].GetAddressOf()), "RasterizerState creation failed.");
		}


		// SamplerState
		for (int i = 0; i < (uint32)eSamplerState::Count; ++i)
		{
			D3D11_SAMPLER_DESC desc = {};
			desc.Filter             = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU           = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV           = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW           = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.ComparisonFunc     = D3D11_COMPARISON_NEVER;
			desc.MinLOD             = 0;
			desc.MaxLOD             = D3D11_FLOAT32_MAX;

			switch ((eSamplerState)i)
			{
			case crab::eSamplerState::PointWrap:
				desc.Filter   = D3D11_FILTER_MIN_MAG_MIP_POINT;
				desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				break;

			case crab::eSamplerState::PointClamp:
				desc.Filter   = D3D11_FILTER_MIN_MAG_MIP_POINT;
				desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
				break;

			case crab::eSamplerState::LinearWrap:
				desc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				break;

			case crab::eSamplerState::LinearClamp:
				desc.Filter   = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
				break;

			case crab::eSamplerState::AnisotropicWrap:
				desc.Filter        = D3D11_FILTER_ANISOTROPIC;
				desc.AddressU      = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressV      = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.AddressW      = D3D11_TEXTURE_ADDRESS_WRAP;
				desc.MaxAnisotropy = 16;
				break;

			case crab::eSamplerState::AnisotropicClamp:
				desc.Filter        = D3D11_FILTER_ANISOTROPIC;
				desc.AddressU      = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.AddressV      = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.AddressW      = D3D11_TEXTURE_ADDRESS_CLAMP;
				desc.MaxAnisotropy = 16;
				break;

			default:
				break;
			}

			DX_ASSERT(device->CreateSamplerState(&desc, m_samplerStates[i].GetAddressOf()), "SamplerState creation failed.");
		}

	}
}

