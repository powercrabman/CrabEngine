#pragma once

namespace crab
{
	class ViewportPanel;
	class EntityInspectorPanel;
	class SceneHierarchyPanel;
	class AssetBrowserPanel;

	struct EditorConfig
	{
		int32 editorAppPosX = 100.f;
		int32 editorAppPosY = 50.f;
		int32 editorAppWidth = 1600;
		int32 editorAppHeight = 900;

		std::filesystem::path stylePath = GetAssetManager().GetEngineDirectory(R"(Configs\EditorStyle.json)");

		SERIALIZERALBE_INTRUSIVE(EditorConfig, editorAppPosX, editorAppPosY, editorAppWidth, editorAppHeight, stylePath);
	};

	class Editor
	{
	public:
		Editor();
		~Editor();

		void Init(const ApplicationSetting& in_setting);

		void OnUpdate(TimeStamp& in_ts);
		void OnImGuiRender(TimeStamp& in_ts);

		void OnEvent(IEvent& in_event);

	private:
		void _handle_input_(TimeStamp& in_ts) const;
		void _load_imgui_style_();
		void _default_editor_imgui_style_();

		std::filesystem::path	m_configPath = GetAssetManager().GetClientDirectory("Config\\EditorConfigs.json");

		// Config
		EditorConfig	m_config;
		bool			m_showStyleEditor = false;
		bool		 	m_showDemoWindow = false;

		// Panels
		Scope<ViewportPanel>		m_viewportPanel;
		Scope<EntityInspectorPanel>	m_entityInspector;
		Scope<SceneHierarchyPanel>	m_sceneHierarchy;
		Scope<AssetBrowserPanel>	m_assetBrowser;
	};
}
