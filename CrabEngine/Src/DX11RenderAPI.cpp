#include "CrabEnginePch.h"
#include "DX11RenderAPI.h"
#include "DX11RenderTarget.h"
#include "IRenderState.h"
#include "DX11RenderState.h"
#include "Geometry.h"
#include "IDepthStencil.h"
#include "ITexture.h"

namespace crab
{
	void DX11RenderAPI::Init(const ApplicationSetting& in_setting)
	{
		HR hr = {};

		// DX device initialize
		uint32 createDeviceFlags = 0;

#if defined(_DEBUG) && defined(CRAB_GFX_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		static D3D_FEATURE_LEVEL sFeatureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = in_setting.windowWidth;
		swapChainDesc.BufferDesc.Height = in_setting.windowHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
		swapChainDesc.OutputWindow = static_cast<HWND>(CrabEngine::Get().GetWindow().GetNativeWindow());
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.Windowed = TRUE;

		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			sFeatureLevels,
			ARRAYSIZE(sFeatureLevels),
			D3D11_SDK_VERSION,
			&swapChainDesc,
			m_swapchain.GetAddressOf(),
			m_device.GetAddressOf(),
			&m_featureLevel,
			m_context.GetAddressOf()
		);
		DX_ASSERT(hr, "Create device fail");

#if defined(_DEBUG) && defined(CRAB_GFX_DEBUG)
		DX_ASSERT(m_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(m_debug.GetAddressOf())), "Create Debug Layer Fail.");
		DX_ASSERT(m_device->QueryInterface(__uuidof(ID3D11InfoQueue), reinterpret_cast<void**>(d3dInfoQueue.GetAddressOf())), "Create Info Queue Fail.");

		d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
#endif

		// base render target creation
		RenderFrameProp prop = {};
		prop.renderTargets = { DX11RenderTarget::CreateBySwapChain() };
		prop.depthStencil = IDepthStencil::Create(in_setting.windowWidth, in_setting.windowHeight);
		m_baseRenderFrame = IRenderFrame::Create(prop);
		m_curRenderFrame = m_baseRenderFrame;

		// render state creation
		m_renderState = MakeScope<DX11RenderState>();

		// constant buffer
		m_transformCBuffer = IConstantBuffer::Create<CBTransform>();
		m_transformCBuffer->Bind(CBTransform::s_bindFlags, CBTransform::s_slot);

		m_cameraCBuffer = IConstantBuffer::Create<CBCamera>();
		m_cameraCBuffer->Bind(CBCamera::s_bindFlags, CBCamera::s_slot);

		m_effectCBuffer = IConstantBuffer::Create<CBEffect>();
		m_effectCBuffer->Bind(CBEffect::s_bindFlags, CBEffect::s_slot);
	}

	void DX11RenderAPI::Shutdown()
	{
	}

	crab::eRenderAPI DX11RenderAPI::GetAPIType()
	{
		return eRenderAPI::DirectX11;
	}

	void DX11RenderAPI::CreateSwapChain(void* in_windowHandle, const int in_width, const int in_height)
	{
		// use create DeviceAndSwapChain
		// not implement this
	}

	void DX11RenderAPI::OnResize(int in_width, int in_height)
	{
		m_curRenderFrame->UnBind();

		bool isSameRenderFrame = (m_baseRenderFrame == m_curRenderFrame);
		if (isSameRenderFrame) m_curRenderFrame.reset();
		m_baseRenderFrame.reset();

		DX_ASSERT(m_swapchain->ResizeBuffers(0, in_width, in_height, DXGI_FORMAT_UNKNOWN, 0), "ResizeBuffers fail");

		RenderFrameProp prop = {};
		prop.renderTargets = { DX11RenderTarget::CreateBySwapChain() };
		prop.depthStencil = IDepthStencil::Create(in_width, in_height);
		m_baseRenderFrame = IRenderFrame::Create(prop);

		if (isSameRenderFrame)
		{
			m_curRenderFrame = m_baseRenderFrame;
			m_curRenderFrame->Bind();
		}
	}

	void DX11RenderAPI::Present()
	{
		m_swapchain->Present(0, 0);
	}

	void DX11RenderAPI::DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_worldMat)
	{
		in_geometry->Bind();
		m_transformCBuffer->SetData(CBTransform{ in_worldMat });
		m_context->DrawIndexed(in_geometry->GetIndexCount(), 0, 0);
	}

	void DX11RenderAPI::DrawIndices(const Ref<IVertexBuffer>& in_vb, const Ref<IIndexBuffer>& in_ib, const Mat& in_transform)
	{
		in_vb->Bind();
		in_ib->Bind();

		m_transformCBuffer->SetData(in_transform);
		m_context->DrawIndexed(in_ib->GetIndexCount(), 0, 0);
	}

	void DX11RenderAPI::SetRenderFrame(const Ref<IRenderFrame>& in_renderframe)
	{
		if (in_renderframe)
		{
			m_curRenderFrame = in_renderframe;
		}
		else
		{
			m_curRenderFrame = m_baseRenderFrame;
		}

		m_curRenderFrame->Bind();
	}

	void DX11RenderAPI::SetCameraData(const Mat& in_viewPorjMat, const Vec3& in_cameraPos)
	{
		m_viewProjMat = in_viewPorjMat;
		m_cameraPosition = in_cameraPos;
	}

	void DX11RenderAPI::BindBlendState(eBlendState in_state)
	{
		if (in_state != m_pipeline.blendState)
		{
			m_pipeline.blendState = in_state;
			m_renderState->BindBlendState(in_state);
		}
	}

	void DX11RenderAPI::BindDepthStencilState(eDepthStencilState in_state)
	{
		if (in_state != m_pipeline.depthStencilState)
		{
			m_pipeline.depthStencilState = in_state;
			m_renderState->BindDepthStencilState(in_state);
		}
	}

	void DX11RenderAPI::BindRasterizerState(eRasterizerState in_state)
	{
		if (in_state != m_pipeline.rasterizerState)
		{
			m_pipeline.rasterizerState = in_state;
			m_renderState->BindRasterizerState(in_state);
		}
	}

	void DX11RenderAPI::BindSamplerState(eSamplerState in_state, uint32 in_slot)
	{
		if (in_state != m_pipeline.samplerState)
		{
			m_pipeline.samplerState = in_state;
			m_renderState->BindSamplerState(in_state, in_slot);
		}
	}

	void DX11RenderAPI::BindTexture(const uint32 in_slot, const Ref<ITexture>& in_texture)
	{
		if (m_pipeline.texture != in_texture)
		{
			m_pipeline.texture = in_texture;
			auto* srv = static_cast<ID3D11ShaderResourceView*>(m_pipeline.texture->GetNativeTexture());
			m_context->PSSetShaderResources(in_slot, 1, &srv);
			SetEffectData({ true });
		}
	}

	void DX11RenderAPI::UnbindTexture(const uint32 in_slot)
	{
		if (m_pipeline.texture)
		{
			SetEffectData(CBEffect{ false });
			m_pipeline.texture = nullptr;
		}
	}

	void DX11RenderAPI::BindShader(const Ref<IShader>& in_shader)
	{
		if (m_pipeline.shader != in_shader)
		{
			m_pipeline.shader = in_shader;
			auto* layout = static_cast<ID3D11InputLayout*>(in_shader->GetNativeLayout());
			auto* vs = static_cast<ID3D11VertexShader*>(in_shader->GetNativeVS());
			auto* ps = static_cast<ID3D11PixelShader*>(in_shader->GetNativePS());

			m_context->IASetInputLayout(layout);
			m_context->VSSetShader(vs, nullptr, 0);
			m_context->PSSetShader(ps, nullptr, 0);
		}
	}

	void DX11RenderAPI::BindRenderFrame() const
	{
		m_curRenderFrame->Bind();
	}

	void DX11RenderAPI::ClearRenderFrame(bool in_clearDepth, bool in_clearStencil) const
	{
		m_curRenderFrame->Clear(in_clearDepth, in_clearStencil);
	}

	void DX11RenderAPI::SetEffectData(const CBEffect& in_data)
	{
		m_effectCBuffer->SetData(in_data);
	}

	void DX11RenderAPI::RenderBegin()
	{
		// Primitive Topology
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Camera
		m_cameraCBuffer->SetData(CBCamera{ m_viewProjMat, m_cameraPosition });
	}

	void DX11RenderAPI::EndRender()
	{
	}
}
