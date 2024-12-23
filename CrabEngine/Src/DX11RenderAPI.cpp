#include "CrabEnginePch.h"
#include "DX11RenderAPI.h"
#include "DX11RenderTarget.h"
#include "IRenderState.h"
#include "DX11RenderState.h"
#include "Geometry.h"

namespace crab
{
	DX11RenderAPI::DX11RenderAPI() {}
	DX11RenderAPI::~DX11RenderAPI() {}

	void DX11RenderAPI::Init(const int in_width, const int in_height)
	{
		HR hr = {};

		// DX device initialize
		uint32 createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
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
		swapChainDesc.BufferDesc.Width = in_width;
		swapChainDesc.BufferDesc.Height = in_height;
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

		// render target creation
		m_coreRenderTarget = DX11RenderTarget::CreateBySwapChain();
		BindRenderTarget(m_coreRenderTarget);

		// render state creation
		m_renderState = MakeScope<DX11RenderState>();

		// constant buffer
		m_transformBuffer = IConstantBuffer::Create<CBTransform>();
		m_transformBuffer->Bind(eShaderFlags_VertexShader, eConstantBufferSlot::TransformSlot);

		m_cameraBuffer = IConstantBuffer::Create<CBCamera>();
		m_cameraBuffer->Bind(eShaderFlags_VertexShader, eConstantBufferSlot::CameraSlot);
	}

	void DX11RenderAPI::Shutdown()
	{
	}

	crab::eRenderAPI DX11RenderAPI::GetAPIType()
	{
		return eRenderAPI::DirectX11;
	}

	void DX11RenderAPI::CreateSwapChain(void* in_windowHanle, const int in_width, const int in_height)
	{
		// use create DeviceAndSwapChain
		// not implement this
	}

	void DX11RenderAPI::OnResize(int in_width, int in_height)
	{
		m_context->OMSetRenderTargets(0, nullptr, nullptr);
		m_context->RSSetViewports(0, nullptr);

		m_coreRenderTarget.reset();
		m_currentRenderTarget.reset();

		DX_ASSERT(m_swapchain->ResizeBuffers(0, in_width, in_height, DXGI_FORMAT_UNKNOWN, 0), "ResizeBuffers fail");

		m_coreRenderTarget = DX11RenderTarget::CreateBySwapChain();
		BindRenderTarget(m_coreRenderTarget);
	}

	void DX11RenderAPI::Present()
	{
		m_swapchain->Present(0, 0);
	}

	void DX11RenderAPI::ClearBuffer(const Vec4& in_color, bool in_clearRenderTarget /*= true*/, bool in_clearDepthStencil /*= true*/)
	{
		m_currentRenderTarget->Clear(in_color, in_clearRenderTarget, in_clearDepthStencil);
	}

	void DX11RenderAPI::DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_worldMat)
	{
		in_geometry->Bind();
		m_transformBuffer->SetData(CBTransform{ in_worldMat });
		m_context->DrawIndexed(in_geometry->GetIndexCount(), 0, 0);
	}

	void DX11RenderAPI::BindRenderTarget(const Ref<IRenderTarget>& in_renderTarget)
	{
		Ref<DX11RenderTarget> rtv = std::static_pointer_cast<DX11RenderTarget>(in_renderTarget);
		if (rtv == m_currentRenderTarget) { return; }

		if (rtv)
		{
			m_currentRenderTarget = rtv;
		}
		else
		{
			m_currentRenderTarget = m_coreRenderTarget;
		}

		D3D11_VIEWPORT viewport = m_currentRenderTarget->GetViewport();
		m_context->RSSetViewports(1, &viewport);
		m_context->OMSetRenderTargets(1, m_currentRenderTarget->GetRenderTargetView().GetAddressOf(), m_currentRenderTarget->GetDepthStencilView().Get());
	}

	Ref<IRenderTarget> DX11RenderAPI::GetRenderTarget() const
	{
		return m_currentRenderTarget;
	}

	void DX11RenderAPI::BindBlendState(eBlendState in_state)
	{
		m_renderState->BindBlendState(in_state);
	}

	void DX11RenderAPI::BindDepthStencilState(eDepthStencilState in_state)
	{
		m_renderState->BindDepthStencilState(in_state);
	}

	void DX11RenderAPI::BindRasterizerState(eRasterizerState in_state)
	{
		m_renderState->BindRasterizerState(in_state);
	}

	void DX11RenderAPI::BindSamplerState(eSamplerState in_state, uint32 in_slot)
	{
		m_renderState->BindSamplerState(in_state, in_slot);
	}

	void DX11RenderAPI::RenderBegin()
	{
		// Primitive Topology
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Camera
		m_cameraBuffer->SetData(m_cameraData);
	}

	void DX11RenderAPI::RenderEnd()
	{
	}
}
