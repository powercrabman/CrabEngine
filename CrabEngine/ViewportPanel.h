#pragma once

namespace crab
{
	struct CrabEditorData;

	class ViewportPanel
	{
	public:
		 ViewportPanel();
		 ~ViewportPanel() = default;

		void OnImGuiRender(TimeStamp& in_ts);

	private:
		void _handle_input_(TimeStamp& in_ts);

		void _draw_play_button_() const;
		bool _draw_simul_button_() const;
		void _draw_pause_button_() const;
		void _draw_reset_button_() const;
		void _draw_next_button_(const TimeStamp& in_ts) const;

		ImVec2				m_buttonSize = { 30.f, 30.f };
		ImVec2				m_viewportScreenPos;
		ImVec2				m_viewportSize;
		bool				m_drawEditCmrInspector = false;

		ImGuizmo::MODE		m_guizmoMode = ImGuizmo::WORLD;
	
		Ref<IRenderTarget>			m_editRenderTarget;
		Ref<IUnorderedAccessView>	m_pickingView;
	};
}