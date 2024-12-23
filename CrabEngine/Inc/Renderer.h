#pragma once
#include "IRenderAPI.h"

namespace crab
{
	class Renderer
	{
	public:
		static void					Init(eRenderAPI in_api, const int in_width, const int in_height);
		static void					Shutdown();

		static IRenderAPI*			GetRenderAPI() { return m_api.get(); }
		static eRenderAPI			GetRednerAPIType() { return m_api->GetAPIType(); }

		static void					OnEvent(IEvent& in_event);
		static void					Present() { m_api->Present(); }

		static void					RenderBegin() { m_api->RenderBegin(); }
		static void					RenderEnd() { m_api->RenderEnd(); }

		static void					ClearBuffer(const Vec4& in_color, bool in_clearRenderTarget = true, bool in_clearDepthStencil = true);;
		static void					DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_transform = Mat::Identity);

		static void					BindBlendState(eBlendState in_state) { m_api->BindBlendState(in_state); }
		static void					BindDepthStencilState(eDepthStencilState in_state) { m_api->BindDepthStencilState(in_state); }
		static void					BindRasterizerState(eRasterizerState in_state) { m_api->BindRasterizerState(in_state); };
		static void					BindSamplerState(eSamplerState in_state, uint32 in_slot = 0) { m_api->BindSamplerState(in_state, in_slot); }

		static void					SetCameraData(const CBCamera& in_data) { m_api->SetCameraData(in_data); }

		static void					BindRenderTarget(const Ref<IRenderTarget>& in_renderTarget) { m_api->BindRenderTarget(in_renderTarget); }
		static Ref<IRenderTarget>	GetRenderTarget() { return m_api->GetRenderTarget(); }

	private:

		inline static Scope<IRenderAPI> m_api = nullptr;
	};

	//===================================================
	//                      Inline
	//===================================================

	inline void Renderer::ClearBuffer(const Vec4& in_color, bool in_clearRenderTarget, bool in_clearDepthStencil)
	{
		m_api->ClearBuffer(in_color, in_clearRenderTarget, in_clearDepthStencil);
	}

	inline void Renderer::DrawIndices(const Ref<Geometry>& in_geometry, const Mat& in_transform)
	{
		m_api->DrawIndices(in_geometry, in_transform);
	}
}