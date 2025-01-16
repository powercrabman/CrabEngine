#include "CrabEnginePch.h"
#include "Editor.h"
#include "IRenderTarget.h"
#include "ViewportPanel.h"
#include "EntityInspectorPanel.h"
#include "SceneHierarchyPanel.h"
#include "ImGuiRenderer.h"
#include "AssetBrowserPanel.h"
#include "EditorEvent.h"
#include "CrabEditorData.h"
#include "IDepthStencil.h"
#include "IUnorderedAccessView.h"
#include "SceneSerializer.h"
#include "VisualLogPanel.h"
#include "ImGuizmo/ImGuizmo.h"

namespace crab
{
	Editor::Editor()
	{
		// 설정파일
		std::filesystem::create_directories(m_configPath.parent_path());

		// 설정가져오기
		if (std::filesystem::exists(m_configPath))
		{
			JsonSerializer<EditorConfig> json;
			json.LoadJsonFromFile(m_configPath);
			m_config = json.LoadFromJson();
		}

		GameWindow& window = GetEngine().GetWindow();
		window.ResizeWindow(m_config.editorAppWidth, m_config.editorAppHeight);
		window.MoveWindow(m_config.editorAppPosX, m_config.editorAppPosY);
	}

	Editor::~Editor()
	{
		JsonSerializer<EditorConfig> json;
		json.SaveToJson(m_config);
		json.SaveJsonToFile(m_configPath);
	}

	void Editor::Init(const ApplicationSetting& in_setting)
	{
		CrabEditorData& gData = GetCrabEditorData();

		RenderTargetProp renderTargetProp = {};
		renderTargetProp.width = in_setting.windowWidth;
		renderTargetProp.height = in_setting.windowHeight;
		renderTargetProp.maxDepth = 1;
		renderTargetProp.minDepth = 0;
		renderTargetProp.clearColor = Color::BLACK;

		Ref<IRenderTarget> renderTarget = IRenderTarget::Create(renderTargetProp);

		UnorderedAccessViewProp uavProp = {};
		uavProp.width = in_setting.windowWidth;
		uavProp.height = in_setting.windowHeight;
		uavProp.format = eUnorderedAccessViewDataFormat::UInt;
		uavProp.accessType = eUnorderedAccessViewAccessType::Read;
		uavProp.clearValue.SetUInts({ UINT32_LIMIT,UINT32_LIMIT ,UINT32_LIMIT ,UINT32_LIMIT });

		Ref<IUnorderedAccessView> uav = IUnorderedAccessView::Create(uavProp);

		RenderFrameProp renderFrameProp = {};
		renderFrameProp.renderTargets = { renderTarget };
		renderFrameProp.depthStencil = IDepthStencil::Create(in_setting.windowWidth, in_setting.windowHeight);
		renderFrameProp.unorderedAccessViews = { uav };

		gData.editRenderFrame = IRenderFrame::Create(renderFrameProp);

		// Load Font
		ImGuiIO& io = ImGui::GetIO();
		gData.editorFont = io.Fonts->AddFontFromFileTTF(GetAssetManager().GetEngineDirectory("Fonts\\GodoM.ttf").string().c_str(), 18.f, nullptr, io.Fonts->GetGlyphRangesKorean());
		gData.editorBoldFont = io.Fonts->AddFontFromFileTTF(GetAssetManager().GetEngineDirectory("Fonts\\GodoB.ttf").string().c_str(), 18.f, nullptr, io.Fonts->GetGlyphRangesKorean());

		// Load Texture
		gData.brandIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\engine_icon.png"));
		gData.playIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\play_button.png"));
		gData.simulateIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\simul_button.png"));
		gData.pauseIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\pause_button.png"));
		gData.resetIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\reset_button.png"));
		gData.nextFrameIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\next_button.png"));
		gData.cameraIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\camera_icon.png"));
		gData.translateIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\move_icon.png"));
		gData.rotateIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\rotation_icon.png"));
		gData.scaleIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\scaling_icon.png"));
		gData.gearIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\gear_icon.png"));
		gData.tagIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\tag_icon.png"));
		gData.transformIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\transform_icon.png"));
		gData.spriteIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\sprite_icon.png"));
		gData.plusIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\plus_icon.png"));
		gData.flipbookIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\flipbook_icon.png"));
		gData.meshIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\mesh_icon.png"));
		gData.monoScriptIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\mono_script_icon.png"));
		gData.docIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\doc_icon.png"));
		gData.directoryIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\folder_icon.png"));
		gData.refreshIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\refresh_icon.png"));
		gData.searchIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\search_icon.png"));
		gData.pencilIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\pencil_icon.png"));
		gData.checkerBoard = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\Checkerboard.png"));
		gData.errorIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\error_icon.png"));
		gData.warnIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\warn_icon.png"));
		gData.infoIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\info_icon.png"));
		gData.trashIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\trash_icon.png"));
		gData.saveIcon = ITexture::Create(GetAssetManager().GetEngineDirectory("Icons\\save_icon.png"));

		// Load Panel
		m_viewportPanel = MakeScope<ViewportPanel>();
		m_entityInspector = MakeScope<EntityInspectorPanel>();
		m_sceneHierarchy = MakeScope<SceneHierarchyPanel>();
		m_assetBrowser = MakeScope<AssetBrowserPanel>();
		m_visualLog = MakeScope<VisualLogPanel>();

		// Load edit scene
		Scene* sc = GetSceneManager().TryFindSceneByName(m_config.editSceneName);
		if (sc) { GetSceneManager().ChangeScene(sc); }

		// Load editor style
		load_imgui_style();
	}

	void Editor::default_editor_imgui_style()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.22f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.99f, 0.90f, 0.40f, 0.25f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.99f, 0.90f, 0.40f, 0.51f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.36f, 0.32f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.22f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.96f, 0.95f, 1.00f, 0.29f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.96f, 0.95f, 1.00f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.99f, 0.90f, 0.40f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.99f, 0.90f, 0.40f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.99f, 0.94f, 0.65f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.00f, 0.43f, 0.37f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.58f, 0.52f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.27f, 0.70f, 0.64f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.00f, 0.43f, 0.37f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.58f, 0.52f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.70f, 0.64f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.99f, 0.92f, 0.55f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.99f, 0.90f, 0.40f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.27f, 0.27f, 0.29f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.99f, 0.90f, 0.40f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.99f, 0.96f, 0.81f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.63f, 0.57f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.09f, 0.36f, 0.32f, 1.00f);
		colors[ImGuiCol_TabSelected] = ImVec4(0.08f, 0.51f, 0.45f, 1.00f);
		colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_TabDimmed] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.99f, 0.38f, 0.55f, 0.71f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.99f, 0.38f, 0.55f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.99f, 0.90f, 0.40f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.99f, 0.38f, 0.55f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.61f, 0.72f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.99f, 0.38f, 0.55f, 1.00f);
		colors[ImGuiCol_NavCursor] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.AntiAliasedFill = true;
		style.AntiAliasedLines = true;
		style.AntiAliasedLinesUseTex = true;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.CellPadding = ImVec2(4.0f, 2.0f);
		style.ChildBorderSize = 1.0f;
		style.ChildRounding = 0.0f;
		style.CircleTessellationMaxError = 0.3f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ColumnsMinSpacing = 6.0f;
		style.CurveTessellationTol = 1.25f;
		style.DisabledAlpha = 0.6f;
		style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
		style.DisplayWindowPadding = ImVec2(20.0f, 19.0f);
		style.DockingSeparatorSize = 2.0f;
		style.FrameBorderSize = 0.0f;
		style.FramePadding = ImVec2(14.0f, 5.0f);
		style.FrameRounding = 0.0f;
		style.GrabMinSize = 20.0f;
		style.GrabRounding = 0.0f;
		style.HoverDelayNormal = 0.4f;
		style.HoverDelayShort = 0.15f;
		style.HoverStationaryDelay = 0.15f;
		style.IndentSpacing = 21.0f;
		style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 8.0f);
		style.LogSliderDeadzone = 4.0f;
		style.MouseCursorScale = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.ScrollbarSize = 20.0f;
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
		style.SeparatorTextAlign = ImVec2(0.0f, 0.5f);
		style.SeparatorTextBorderSize = 3.0f;
		style.SeparatorTextPadding = ImVec2(20.0f, 3.0f);
		style.TabBarBorderSize = 2.0f;
		style.TabBarOverlineSize = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.TabRounding = 0.0f;
		style.TableAngledHeadersAngle = 0.6108652353286743f;
		style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
		style.WindowBorderSize = 1.0f;
		style.WindowMenuButtonPosition = ImGuiDir_Left;
		style.WindowMinSize = ImVec2(32.0f, 32.0f);
		style.WindowPadding = ImVec2(8.0f, 8.0f);
		style.WindowRounding = 0.0f;
		style.WindowTitleAlign = ImVec2(0.0f, 0.5f);

		JsonSerializer<ImGuiStyle> json;
		json = ImGui::GetStyle();
		json.SaveJsonToFile(m_config.stylePath);
	}

	void Editor::handle_input(TimeStamp& in_ts) const
	{
		CrabEditorData& gData = GetCrabEditorData();
		if (gData.editorState == eEditorState::Edit)
		{
			// delete
			if (Input::IsKeyDown(eKey::Delete))
			{
				DeleteEntity_EditorEvent e;
				e.m_entityID = gData.selectedEntity.GetID();
				GetEngine().DispatchEvent(e);
			}

			// duplicate
			if (Input::IsKeyDown(eKey::LeftCtrl) && Input::IsKeyPress(eKey::D))
			{
				DuplicateEntity_EditorEvent e;
				e.m_entityID = gData.selectedEntity.GetID();
				GetEngine().DispatchEvent(e);
			}

			// Scene
			if (Input::IsKeyDown(eKey::LeftCtrl) && Input::IsKeyPress(eKey::S))
			{
				SaveProject_EditorEvent e;
				GetEngine().DispatchEvent(e);
			}
		}

		if (gData.editorState == eEditorState::SimulatePlay)
		{
			if (Input::IsKeyPress(eKey::Space))
			{
				SetEditorState_EditorEvent e;
				e.m_editorState = eEditorState::SimulateStop;
				GetEngine().DispatchEvent(e);
			}

			if (Input::IsKeyPress(eKey::Escape))
			{
				SetEditorState_EditorEvent e;
				e.m_editorState = eEditorState::Edit;
				GetEngine().DispatchEvent(e);
			}
		}

		if (gData.editorState == eEditorState::SimulateStop)
		{
			if (Input::IsKeyPress(eKey::Space, true))
			{
				NextFrame_EditorEvent e;
				e.m_timeStamp = in_ts;
				GetEngine().DispatchEvent(e);
			}

			if (Input::IsKeyPress(eKey::Escape))
			{
				SetEditorState_EditorEvent e;
				e.m_editorState = eEditorState::Edit;
				GetEngine().DispatchEvent(e);
			}
		}

		if (gData.editorState == eEditorState::Runtime)
		{
			if (Input::IsKeyPress(eKey::Escape))
			{
				SetEditorState_EditorEvent e;
				e.m_editorState = eEditorState::Edit;
				GetEngine().DispatchEvent(e);
			}
		}
	}

	void Editor::load_imgui_style()
	{
		if (std::filesystem::exists(m_config.stylePath))
		{
			JsonSerializer<ImGuiStyle> json;
			if (json.LoadJsonFromFile(m_config.stylePath))
			{
				ImGui::GetStyle() = json.LoadFromJson();
			}
			else
			{
				default_editor_imgui_style();
			}
		}
		else
		{
			default_editor_imgui_style();
		}
	}

	void Editor::OnUpdate(TimeStamp& in_ts)
	{
		CrabEditorData& gData = GetCrabEditorData();
		switch (gData.editorState)
		{
		case crab::eEditorState::Runtime:
			SceneManager::Get().OnUpdate(in_ts);
			break;

		case crab::eEditorState::SimulatePlay:
			SceneManager::Get().OnUpdate(in_ts);
			[[fallthrough]];

		case crab::eEditorState::SimulateStop:
		case crab::eEditorState::Edit:
		{
			Renderer::SetCameraData(
				gData.editorCamera.GetViewProj(
					gData.editRenderFrame->GetRenderTarget(0)->GetAspect()
				),
				gData.editorCamera.transform.position
			);
		}
		break;

		default: assert(false);
		}

		// 렌더링
		Renderer::BeginRender();
		Renderer::BindRenderFrame(gData.editRenderFrame);
		Renderer::ClearRenderFrame();

		// main rendering 
		Draw2D::BeginBatch();
		SceneManager::Get().OnRender(in_ts);
		Draw2D::EndBatch();

		// GUI 렌더링
		ImGuiRenderer::BeginRender();
		Renderer::ResetRenderFrame();
		Renderer::ClearRenderFrame();
		OnImGuiRender(in_ts);
		SceneManager::Get().OnImGuiRender(in_ts);
		ImGuiRenderer::EndRender();

		// 렌더링 종료
		Renderer::EndRender();
		Renderer::Present();
	}

	void Editor::OnImGuiRender(TimeStamp& in_ts)
	{
		CrabEditorData& gData = GetCrabEditorData();
		ImFontStyle font{ gData.editorFont };

		handle_input(in_ts);

#pragma region DockSpace
		// DockSpace
		{
			ImDisable disable{ gData.editorState != eEditorState::Edit };

			// Menu bar
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking |
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoBringToFrontOnFocus |
				ImGuiWindowFlags_NoNavFocus;

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);

			// Dock space
			{
				ImStyle s0(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				ImStyle s1(ImGuiStyleVar_WindowRounding, 0.f);
				ImGui::Begin("DockSpace", nullptr, window_flags);
			}

			ImGuiStyle& style = ImGui::GetStyle();
			ImGuiContext* c = ImGui::GetCurrentContext();
			ImDrawList* draw = ImGui::GetWindowDrawList();
			constexpr float MENUBAR_HEIGHT = 48.f;
			static bool s_infoModalEnable = false;

			// Logo
			{
				ImTextureID id = reinterpret_cast<ImTextureID>(gData.brandIcon->GetNativeTexture());
				ImVec2 size = ImVec2{ MENUBAR_HEIGHT ,MENUBAR_HEIGHT } - style.ItemSpacing;
				ImGui::Dummy(size);
				ImVec2 p0 = ImGui::GetWindowPos() + style.ItemSpacing;
				ImVec2 p1 = p0 + size;
				draw->AddImage(id, p0, p1);
			}

			// Text
			{
				ImFontStyle font{ gData.editorBoldFont };
				ImVec2 p0 = {};
				p0.x = ImGui::GetWindowPos().x + viewport->WorkSize.x - ImGui::CalcTextSize(GetEngine().GetApplicationName()).x - 20.f;
				p0.y = ImGui::GetWindowPos().y + (MENUBAR_HEIGHT - c->FontSize) * 0.5f;
				draw->AddText(p0, ImGui::GetColorU32(ImGuiCol_Text), GetEngine().GetApplicationName());
			}

			// Menu Item
			{
				ImStyle s0{ ImGuiStyleVar_FrameRounding, 0.f };
				ImColorStyle c0{ ImGuiCol_Button, ImVec4{0.f,0.f,0.f,0.f} };
				ImColorStyle c1{ ImGuiCol_ButtonHovered, ImVec4{1.f,1.f,1.f,0.2f} };
				ImColorStyle c2{ ImGuiCol_ButtonActive, ImVec4{1.f,1.f,1.f,0.4f} };

				ImVec2 p0 = {};
				p0.x = ImGui::GetWindowPos().x + MENUBAR_HEIGHT + style.ItemSpacing.x;
				p0.y = ImGui::GetWindowPos().y + MENUBAR_HEIGHT - style.ItemSpacing.y - c->FontSize - style.FramePadding.y * 2.f;
				ImGui::SetCursorScreenPos(p0);

				ImGui::Button("File");
				if (ImPopupItem item{})
				{
					if (ImGui::MenuItem("Save Project", "Ctrl+S"))
					{
						SaveProject_EditorEvent e;
						GetEngine().DispatchEvent(e);
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Save Current Scene"))
					{
						if (Scene* sc = GetSceneManager().TryGetCurrentScene())
						{
							SaveScene_EditorEvent e;
							e.m_scene = sc;
							GetEngine().DispatchEvent(e);
						}
					}

					if (ImGui::MenuItem("Restart Current Scene"))
					{
						if (Scene* sc = GetSceneManager().TryGetCurrentScene())
						{
							RestartScene_EditorEvent e;
							GetEngine().DispatchEvent(e);
						}
					}

					ImGui::Separator();
					if (ImGui::MenuItem("Exit")) { AppClose_Event e; GetEngine().DispatchEvent(e); }
				}

				ImGui::SameLine();
				ImGui::Button("View");
				if (ImPopupItem item{})
				{
					ImGui::MenuItem("Entity Inspector");
					ImGui::MenuItem("Scene Hierarchy");
					ImGui::MenuItem("Asset Browser");
				}

				ImGui::SameLine();
				ImGui::Button("Style");
				if (ImPopupItem item{})
				{
					if (ImGui::MenuItem("Color Style", "", m_showStyleEditor)) { m_showStyleEditor = !m_showStyleEditor; }
					if (ImGui::MenuItem("Demo Window", "", m_showDemoWindow)) { m_showDemoWindow = !m_showDemoWindow; }
				}

				ImGui::SameLine();
				ImGui::Button("Help");
				if (ImPopupItem item{})
				{
					if (ImGui::MenuItem("Information"))
					{
						s_infoModalEnable = true;
					}
				}
			}

			if (s_infoModalEnable) { s_infoModalEnable = false; ImGui::OpenPopup("Information"); }
			if (ImModal modal{ "Information" })
			{
				ImTextureID id = reinterpret_cast<ImTextureID>(gData.brandIcon->GetNativeTexture());
				auto [width, height] = gData.brandIcon->GetSize();
				ImGui::Image(id, { width, height });
				ImGui::SameLine();
				ImGui::TextUnformatted(
					"Hello, Crab Engine! by powercrabman\n"
					"Team: Studio Baekro\n"
					"Version: In Development\n"
				);

				ImGui::TextLinkOpenURL("Github: https://github.com/powercrabman", "https://github.com/powercrabman");
				modal.DrawButton("OK");
			}

			ImGuiID dockspaceID = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspaceID);
			ImGui::End();
		}

#pragma endregion DockSpace

#pragma region Panels
		m_viewportPanel->OnImGuiRender(in_ts);
		ImDisable disable{ gData.editorState != eEditorState::Edit };
		m_entityInspector->OnImGuiRender(in_ts);
		m_sceneHierarchy->OnImGuiRender(in_ts);
		m_assetBrowser->OnImGuiRender(in_ts);
		m_visualLog->OnImGuiRender(in_ts);
#pragma endregion Panels

#pragma region Performence Checker

		{
			ImWindow window{ "Performance" };
			ImGui::Text("Total time: %.2f sec", in_ts.totalTime);
			ImGui::Text("Delta time: %.2f ms", in_ts.deltaTime * 1000.f);
			ImGui::Text("Frame per sec: %.2f per sec", 1.f / in_ts.rawDeltaTime);
		}

#pragma endregion Performence Checker

#pragma region ImGui Demo
		if (m_showStyleEditor)
		{
			ImWindow window{ "Style Editor", &m_showStyleEditor };
			ImGui::ShowStyleEditor();

			if (ImGui::Button("Save Theme"))
			{
				JsonSerializer<ImGuiStyle> json;
				json = ImGui::GetStyle();
				json.SaveJsonToFile(m_config.stylePath);
			}
		}

		if (m_showDemoWindow)
		{
			ImGui::ShowDemoWindow(&m_showDemoWindow);
		}
#pragma endregion ImGui Demo

	}

	void Editor::OnEvent(IEvent& in_event)
	{
		EventDispatcher dispatcher{ in_event };

		CRAB_REGISTER_EVENT_HANDLER(WindowResize_Event,
			[&](WindowResize_Event& in_event)
			{
				m_config.editorAppWidth = in_event.m_width;
				m_config.editorAppHeight = in_event.m_height;
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(WindowMove_Event,
			[&](WindowMove_Event& in_event)
			{
				m_config.editorAppPosX = in_event.m_x;
				m_config.editorAppPosY = in_event.m_y;
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(SetEditorState_EditorEvent,
			[&](SetEditorState_EditorEvent& in_event)
			{
				CrabEditorData& gData = GetCrabEditorData();
				gData.editorState = in_event.m_editorState;

				switch (gData.editorState)
				{
				case eEditorState::Runtime:
				{
					Scene* scene = GetSceneManager().TryGetCurrentScene();
					if (scene)
					{
						gData.tempSceneData.ToJson(scene);
						GetSceneManager().RestartCurrentScene();
					}
				}
				break;

				case eEditorState::Edit:
				{
					Scene* scene = GetSceneManager().TryGetCurrentScene();
					if (scene)
					{
						gData.tempSceneData.LoadSceneDataFromJson(scene);
						GetSceneManager().RestartCurrentScene();
					}
				}
				break;

				case eEditorState::SimulatePlay:
				case eEditorState::SimulateStop:
					break;

				default:
					assert(false);
				}
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(NextFrame_EditorEvent,
			[&](NextFrame_EditorEvent& in_event)
			{
				constexpr float FPS_INV = 1 / 60.f;
				in_event.m_timeStamp.deltaTime = FPS_INV;
				SceneManager::Get().OnUpdate(in_event.m_timeStamp);
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(DeleteEntity_EditorEvent,
			[&](DeleteEntity_EditorEvent& in_e)
			{
				Scene* sc = GetSceneManager().TryGetCurrentScene();
				if (sc)
				{
					Entity e = sc->TryFindByID(in_e.m_entityID);
					if (e.IsValid())
					{
						sc->DeleteEntity(e);
					}
				}
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(DuplicateEntity_EditorEvent,
			[&](DuplicateEntity_EditorEvent& in_e)
			{
				Scene* sc = GetSceneManager().TryGetCurrentScene();
				if (sc)
				{
					Entity e = sc->TryFindByID(in_e.m_entityID);
					if (e.IsValid())
					{
						sc->DuplicateEntity(e);
					}
				}
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(SaveScene_EditorEvent,
			[&](SaveScene_EditorEvent& in_e)
			{
				if (in_e.m_scene)
				{
					SendVisualLog(eVisualLogLevel::Info, "Scene Save Done.");
					SceneSerializer serializer;
					serializer.ToJson(in_e.m_scene);
					serializer.SaveJsonToFile(in_e.m_scene->GetSceneDataPath());
				}
				else
				{
					ASSERT(false, "Do not setting scene to save.");
				}
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(SaveAssets_EditorEvent,
			[&](SaveAssets_EditorEvent& in_e)
			{
				SendVisualLog(eVisualLogLevel::Info, "Assets Save Done.");
				AssetManager::Get().SaveClientAssetsToFile();
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(SaveProject_EditorEvent,
			[&](SaveProject_EditorEvent& in_e)
			{
				Scene* sc = GetSceneManager().TryGetCurrentScene();
				if (sc)
				{
					SendVisualLog(eVisualLogLevel::Info, "Project Save Done.");

					SceneSerializer serializer;
					serializer.ToJson(sc);
					serializer.SaveJsonToFile(sc->GetSceneDataPath());
				}

				AssetManager::Get().SaveClientAssetsToFile();
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(RestartScene_EditorEvent,
			[&](RestartScene_EditorEvent& in_e)
			{
				SendVisualLog(eVisualLogLevel::Info, "Scene Reload Done.");
				Scene* sc = GetSceneManager().TryGetCurrentScene();

				if (sc)
				{
					SceneSerializer serializer;
					serializer.LoadJsonFromFile(sc->GetSceneDataPath());
					serializer.LoadSceneDataFromJson(sc);
					GetSceneManager().RestartCurrentScene();
				}
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(SetGuizmoType_EditorEvent,
			[&](SetGuizmoType_EditorEvent& in_e)
			{
				CrabEditorData& gData = GetCrabEditorData();
				switch (in_e.m_type)
				{
				case eGuizmoType::Translation:
					gData.guizmoOp = ImGuizmo::OPERATION::TRANSLATE;
					break;

				case eGuizmoType::Rotation:
					gData.guizmoOp = ImGuizmo::OPERATION::ROTATE;
					break;

				case eGuizmoType::Scaling:
					gData.guizmoOp = ImGuizmo::OPERATION::SCALE;
					break;

				default:
					assert(false);
				}
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(SendVisualLog_EditorEvent,
			[&](SendVisualLog_EditorEvent& in_e)
			{
				m_visualLog->PushLog(in_e.m_level, in_e.m_logMessage);
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(ChangeScene_EngineEvent,
			[&](ChangeScene_EngineEvent& in_e)
			{
				Scene* sc = GetSceneManager().TryGetCurrentScene();
				if (sc)
				{
					m_config.editSceneName = sc->GetName();
				}
			}
		);
	}
}