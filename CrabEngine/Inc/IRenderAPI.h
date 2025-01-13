#pragma once

namespace crab
{
	class Geometry;
	class IRenderTarget;
	class ITexture;
	class IShader;
	class IVertexBuffer;
	class IIndexBuffer;
	class IRenderFrame;

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
		virtual void				Init(const ApplicationSetting& in_setting) = 0;
		virtual void				Shutdown() = 0;
		virtual eRenderAPI			GetAPIType() = 0;

		virtual void				CreateSwapChain(void* in_windowHandle, const int in_width, const int in_height) = 0;
		virtual void				OnResize(int in_width, int in_height) = 0;
		virtual void				Present() = 0;

		virtual void				RenderBegin() = 0;
		virtual void				EndRender() = 0;

		// Draw
		virtual void				DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_transform) = 0;
		virtual void				DrawIndices(const Ref<IVertexBuffer>& in_vb, const Ref<IIndexBuffer>& in_ib, const Mat& in_transform) = 0;

		// Render Frame
		virtual void				SetRenderFrame(const Ref<IRenderFrame>& in_renderFrame) = 0;
		virtual void				ResetRenderFrame() = 0;
		virtual	void				BindRenderFrame() const = 0;
		virtual void				ClearRenderFrame(bool in_clearDepth, bool in_clearStencil) const = 0;

		// Bind
		virtual void				BindBlendState(eBlendState in_state) = 0;
		virtual void				BindDepthStencilState(eDepthStencilState in_state) = 0;
		virtual void				BindRasterizerState(eRasterizerState in_state) = 0;
		virtual void				BindSamplerState(eSamplerState in_state, uint32 in_slot) = 0;

		virtual void				BindTexture(const uint32 in_slot, const Ref<ITexture>& in_texture) = 0;
		virtual void				UnbindTexture(const uint32 in_slot) = 0;

		virtual void				BindShader(const Ref<IShader>& in_shader) = 0;

		// Uniform Buffer
		virtual void				SetEffectData(const CBEffect& in_data) = 0;
		virtual void				SetCameraData(const Mat& in_viewPorjMat, const Vec3& in_cameraPos) = 0;
	};
}