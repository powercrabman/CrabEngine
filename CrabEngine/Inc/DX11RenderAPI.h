#pragma once
#include "Enums.h"
#include "IRenderAPI.h"
#include "IRenderState.h"
#include "IConstantBuffer.h"
#include "IRenderTarget.h"
#include "IRenderFrame.h"
#include "RenderPipeline.h"

namespace crab
{
	class DX11RenderTarget;
	class IShader;
	class ITexture;

	class DX11RenderAPI : public IRenderAPI
	{
	public:
		void				Init(const ApplicationSetting& in_setting) override;
		void				Shutdown() override;
		eRenderAPI			GetAPIType() override;

		void				CreateSwapChain(void* in_windowHandle, const int in_width, const int in_height) override;
		void				OnResize(int in_width, int in_height) override;
		void				Present() override;

		void				RenderBegin() override;
		void				EndRender() override;

		void				DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_worldMat) override;
		void				DrawIndices(const Ref<IVertexBuffer>& in_vb, const Ref<IIndexBuffer>& in_ib, const Mat& in_transform) override;

		// Render Frame
		void				SetRenderFrame(const Ref<IRenderFrame>& in_renderframe) override;
		void				ResetRenderFrame() override { SetRenderFrame(nullptr); }
		void				BindRenderFrame() const override;
		void				ClearRenderFrame(bool in_clearDepth, bool in_clearStencil) const override;

		// Uniform Buffer
		void				SetEffectData(const CBEffect& in_data) override;
		void				SetCameraData(const Mat& in_viewPorjMat, const Vec3& in_cameraPos) override;

		// Pipeline Data
		void				BindBlendState(eBlendState in_state) override;
		void				BindDepthStencilState(eDepthStencilState in_state) override;
		void				BindRasterizerState(eRasterizerState in_state) override;
		void				BindSamplerState(eSamplerState in_state, uint32 in_slot) override;

		void				BindTexture(const uint32 in_slot, const Ref<ITexture>& in_texture) override;
		void				UnbindTexture(const uint32 in_slot) override;

		void				BindShader(const Ref<IShader>& in_shader) override;

		// Concrete Graphics Device
		::ComPtr<ID3D11Device>			GetDevice() const { return m_device; }
		::ComPtr<ID3D11DeviceContext>	GetContext() const { return m_context; }
		::ComPtr<IDXGISwapChain>		GetSwapchain() const { return m_swapchain; }

	private:
		// Graphics Device
		::ComPtr<ID3D11Device>			m_device;
		::ComPtr<ID3D11DeviceContext>	m_context;
		::ComPtr<IDXGISwapChain>		m_swapchain;
		D3D_FEATURE_LEVEL				m_featureLevel = {};

		// Debug Device
		::ComPtr<ID3D11Debug>			m_debug;
		::ComPtr<ID3D11InfoQueue>		m_infoQueue;

		// Render Frame
		Ref<IRenderFrame>				m_baseRenderFrame;
		Ref<IRenderFrame>				m_curRenderFrame;

		// Uniform Buffer
		Ref<IConstantBuffer>			m_transformCBuffer = nullptr;
		Ref<IConstantBuffer>			m_cameraCBuffer = nullptr;
		Ref<IConstantBuffer>			m_effectCBuffer = nullptr;

		// Camera
		Mat								m_viewProjMat = Mat::Identity;
		Vec3							m_cameraPosition = Vec3::Zero;

		// Pipeline Data
		RenderPipeline					m_pipeline;
		Scope<IRenderState>				m_renderState = nullptr;
	};
}
