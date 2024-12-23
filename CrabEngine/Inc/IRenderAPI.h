#pragma once

namespace crab
{
	class Geometry;
	class IRenderTarget;

	enum class eCullMode
	{
		NoCull,
		Cull_Clockwise,
		Cull_CounterClockwise
	};

	class IRenderAPI
	{
	public:
		IRenderAPI() = default;
		virtual ~IRenderAPI() = default;

		// Core
		virtual void				Init(const int in_width, const int in_height) = 0;
		virtual void				Shutdown() = 0;
		virtual eRenderAPI			GetAPIType() = 0;

		virtual void				CreateSwapChain(void* in_windowHanle, const int in_width, const int in_height) = 0;
		virtual void				OnResize(int in_width, int in_height) = 0;
		virtual void				Present() = 0;

		virtual void				RenderBegin() = 0;
		virtual void				RenderEnd() = 0;

		// Draw
		virtual void				ClearBuffer(const Vec4& in_color, bool in_clearRenderTarget = true, bool in_clearDepthStencil = true) = 0;
		virtual void				DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_transform) = 0;

		// Bind
		virtual void				BindBlendState(eBlendState in_state) = 0;
		virtual void				BindDepthStencilState(eDepthStencilState in_state) = 0;
		virtual void				BindRasterizerState(eRasterizerState in_state) = 0;
		virtual void				BindSamplerState(eSamplerState in_state, uint32 in_slot) = 0;

		virtual void				SetCameraData(const CBCamera& in_data) = 0;

		virtual void				BindRenderTarget(const Ref<IRenderTarget>& in_renderTarget) = 0;
		virtual Ref<IRenderTarget>	GetRenderTarget() const = 0;
	};
}