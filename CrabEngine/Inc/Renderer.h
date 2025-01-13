#pragma once
#include "IRenderAPI.h"

namespace crab
{
	class IVertexBuffer;
	class IIndexBuffer;

	class Renderer
	{
	public:
		// Core
		static void					Init(const ApplicationSetting& in_setting);
		static void					Shutdown();

		static IRenderAPI*			GetRenderAPI() { return m_api.get(); }
		static eRenderAPI			GetRenderAPIType() { return m_api->GetAPIType(); }

		static void					OnEvent(IEvent& in_event);
		static void					Present() { m_api->Present(); }

		static void					BeginRender();
		static void					EndRender();

		// Draw
		static void					DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_transform = Mat::Identity);
		static void					DrawIndices(const Ref<IVertexBuffer>& in_vb, const Ref<IIndexBuffer>& in_ib, const Mat& in_transform = Mat::Identity);

		// Render Frame
		static void					BindRenderFrame(const Ref<IRenderFrame>& in_renderFrame) { m_api->SetRenderFrame(in_renderFrame); }
		static void					ResetRenderFrame() { m_api->ResetRenderFrame(); }
		static void					ClearRenderFrame(bool in_clearDepth = true, bool in_clearStencil = true) { m_api->ClearRenderFrame(in_clearDepth, in_clearStencil); }

		static void					BindBlendState(eBlendState in_state) { m_api->BindBlendState(in_state); }
		static void					BindDepthStencilState(eDepthStencilState in_state) { m_api->BindDepthStencilState(in_state); }
		static void					BindRasterizerState(eRasterizerState in_state) { m_api->BindRasterizerState(in_state); };
		static void					BindSamplerState(eSamplerState in_state, uint32 in_slot = 0) { m_api->BindSamplerState(in_state, in_slot); }

		static void					BindTexture(const uint32 in_slot, const Ref<ITexture>& in_texture) { m_api->BindTexture(in_slot, in_texture); }
		static void					UnbindTexture(const uint32 in_slot) { m_api->UnbindTexture(in_slot); }

		static void					BindShader(const Ref<IShader>& in_shader) { m_api->BindShader(in_shader); }

		static void					SetEffectData(const CBEffect& in_data) { m_api->SetEffectData(in_data); };
		static void					SetCameraData(const Mat& in_viewPorjMat, const Vec3& in_cameraPos) { m_api->SetCameraData(in_viewPorjMat, in_cameraPos); };

	private:

		inline static Scope<IRenderAPI> m_api = nullptr;
	};

	//===================================================
	//                      Inline
	//===================================================

	inline void Renderer::DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_transform)
	{
		m_api->DrawIndices(in_geometry, in_transform);
	}

	inline void Renderer::DrawIndices(const Ref<IVertexBuffer>& in_vb, const Ref<IIndexBuffer>& in_ib, const Mat& in_worldMat)
	{
		m_api->DrawIndices(in_vb, in_ib, in_worldMat);
	}
}
