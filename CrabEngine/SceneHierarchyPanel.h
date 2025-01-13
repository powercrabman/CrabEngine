#pragma once

namespace crab
{
	struct CrabEditorData;

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();
		void OnImGuiRender(TimeStamp& in_ts);

	private:
		void _draw_scene_menu_();

		std::vector<std::string>	m_sceneNames = {};
		uint32						m_sceneNameIndex = 0;

		bool						m_reloadScModal = false;
	};
}

