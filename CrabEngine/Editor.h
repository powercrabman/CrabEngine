#pragma once

namespace crab
{
	class ViewportPanel;
	class EntityInspectorPanel;
	class SceneHierarchyPanel;
	class AssetBrowserPanel;
	class VisualLogPanel;

	struct EditorConfig
	{
		// Window
		int32 editorAppPosX = 100.f;
		int32 editorAppPosY = 50.f;
		int32 editorAppWidth = 1600;
		int32 editorAppHeight = 900;

		std::string editSceneName = "";

		// Style
		std::filesystem::path stylePath = GetAssetManager().GetEngineDirectory(R"(Configs\EditorStyle.json)");

		// serializer
		friend void to_json(Json& out_json, const EditorConfig& in_config)
		{
			out_json["editor app posX"] = in_config.editorAppPosX;
			out_json["editor app posY"] = in_config.editorAppPosY;
			out_json["editor app width"] = in_config.editorAppWidth;
			out_json["editor app height"] = in_config.editorAppHeight;
			out_json["edit scene name"] = in_config.editSceneName;

			// Serializer Style
			out_json["style path"] = in_config.stylePath;

		}
		friend void from_json(const Json& in_json, EditorConfig& out_config)
		{
			out_config.editorAppPosX = in_json["editor app posX"];
			out_config.editorAppPosY = in_json["editor app posY"];
			out_config.editorAppWidth = in_json["editor app width"];
			out_config.editorAppHeight = in_json["editor app height"];
			out_config.editSceneName = in_json["edit scene name"];

			// Serializer Style
			std::string stylePath = in_json["style path"];
			out_config.stylePath = stylePath;
		}
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
		void handle_input(TimeStamp& in_ts) const;
		void load_imgui_style();
		void default_editor_imgui_style();

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
		Scope<VisualLogPanel>		m_visualLog;
	};
}
