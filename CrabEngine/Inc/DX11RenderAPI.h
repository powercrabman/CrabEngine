#pragma once
#include "IRenderAPI.h"
#include "IRenderState.h"
#include "IConstantBuffer.h"

namespace crab
{
	class DX11RenderTarget;

	class DX11RenderAPI : public IRenderAPI
	{
	public:
		DX11RenderAPI();
		~DX11RenderAPI();

		void				Init(const int in_width, const int in_height) override;
		void				Shutdown() override;
		eRenderAPI			GetAPIType() override;

		void				CreateSwapChain(void* in_windowHanle, const int in_width, const int in_height) override;
		void				OnResize(int in_width, int in_height) override;
		void				Present() override;

		void				RenderBegin() override;
		void				RenderEnd() override;

		void				ClearBuffer(const Vec4& in_color, bool in_clearRenderTarget = true, bool in_clearDepthStencil = true) override;

		void				DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_worldMat) override;

		void				BindBlendState(eBlendState in_state) override;
		void				BindDepthStencilState(eDepthStencilState in_state) override;
		void				BindRasterizerState(eRasterizerState in_state) override;
		void				BindSamplerState(eSamplerState in_state, uint32 in_slot) override;

		void				UnbindTexture(const uint32 in_slot) override;
		void				SetTextureData(const Vec2& in_uv0, const Vec2& in_uv1) override;
		void				SetCameraData(const Mat& in_viewPorjMat, const Vec3& in_cameraPos) override;

		void				BindRenderTarget(const Ref<IRenderTarget>& in_renderTarget) override;
		Ref<IRenderTarget>	GetRenderTarget() const override;


		::ComPtr<ID3D11Device>			GetDevice() const { return m_device; }
		::ComPtr<ID3D11DeviceContext>	GetContext() const { return m_context; }
		::ComPtr<IDXGISwapChain>		GetSwapchain() const { return m_swapchain; }

	private:
		::ComPtr<ID3D11Device>				m_device;
		::ComPtr<ID3D11DeviceContext>		m_context;
		::ComPtr<IDXGISwapChain>			m_swapchain;

		Ref<DX11RenderTarget>				m_coreRenderTarget = nullptr;
		Ref<DX11RenderTarget>				m_currentRenderTarget = nullptr;

		D3D_FEATURE_LEVEL					m_featureLevel = {};

		Scope<IRenderState>					m_renderState = nullptr;

		Ref<IConstantBuffer>				m_transformCBuffer = nullptr;
		Ref<IConstantBuffer>				m_cameraCBuffer = nullptr;
		Ref<IConstantBuffer>				m_textureCBuffer = nullptr;

		// Camera
		Mat									m_viewProjMat = Mat::Identity;
		Vec3								m_cameraPosition = Vec3::Zero;

	};
}