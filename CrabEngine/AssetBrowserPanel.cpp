#include "CrabEnginePch.h"
#include "AssetBrowserPanel.h"

#include "CrabEditorData.h"
#include "PanelDrawHelper.h"

namespace crab
{
	AssetBrowserPanel::AssetBrowserPanel()
	{
		auto& gData = GetCrabEditorData();
		m_iconList.fill(nullptr);
		m_iconList[static_cast<uint32>(eAssetType::Mesh)] = gData.meshIcon;
		m_iconList[static_cast<uint32>(eAssetType::MonoScript)] = gData.monoScriptIcon;
	}

	void AssetBrowserPanel::OnImGuiRender(TimeStamp& in_ts)
	{
		ImGui::SetNextWindowSize({ 500.f, 300 });
		ImWindow window{ "Asset Browser" };
		auto& gData = GetCrabEditorData();

		handle_input();

		{
			ImChildWindow child{ "DirView", {0.2f * ImGui::GetContentRegionAvail().x, 0.f} };

			{
				ImGuiEx::IconRect(ToImTextureID(gData.searchIcon));
				ImGui::SameLine();
				static char s_searchBuf[64] = {};

				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				ImGui::InputTextWithHint("##search", "Search...", s_searchBuf, sizeof(s_searchBuf));
			}

			// Asset Directory
			{
				ImChildWindow child{ "DirTree" , ImVec2{0.f,0.f}, ImGuiChildFlags_Border };

				for (int i = 0; i < eAssetTypeCount; ++i)
				{
					eAssetType type = static_cast<eAssetType>(i);
					if (ImGuiEx::IconSelectable(ToString(type), ToImTextureID(gData.directoryIcon)))
					{
						m_focusAssetType = type;
						m_focusAssetID.Clear();
					}
				}
			}
		}

		ImGui::SameLine();

		// Asset Viewer
		ImGui::BeginGroup();
		{
			auto& gData = GetCrabEditorData();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Zoom Ratio: %.2f", m_browserZoomRatio);

			ImGui::SameLine();
			ImGuiEx::QuestionTooltip(R"(Zoom in/out with "Ctrl + Mouse scroll")");

			ImGui::SameLine();
			if (ImGuiEx::IconTextButton(ToImTextureID(gData.refreshIcon), "Refresh"))
			{
				SendVisualLog(eVisualLogLevel::Info, "Assets refresh done.");
				GetAssetManager().LoadClientAssets();
			}

			ImGui::SameLine();
			ImGui::BeginDisabled(!GetAssetManager().GetDirtyBit());
			if (ImGuiEx::IconTextButton(ToImTextureID(gData.saveIcon), "Save"))
			{
				SendVisualLog(eVisualLogLevel::Info, "Assets save done.");
				GetAssetManager().SaveClientAssetsToFile();
			}
			ImGui::EndDisabled();

			// 에셋 뷰어 출력
			switch (m_focusAssetType)
			{
			case eAssetType::Mesh:
				draw_mesh_browser();
				break;

			case eAssetType::Flipbook:
				draw_flipbook_browser();
				break;

			case eAssetType::Sprite:
				draw_sprite_browser();
				break;

			case eAssetType::MonoScript:
				draw_mono_script_browser();
				break;

			case eAssetType::GameTexture:
				draw_game_texture_browser();
				break;

			default: assert(false);
			}
		}
		ImGui::EndGroup();

		if (m_spriteEditorPanel.IsVisible()) m_spriteEditorPanel.Draw();
		if (m_flipbookEditorPanel.IsVisible()) m_flipbookEditorPanel.Draw(in_ts);
		if (m_monoScriptEditor.IsVisible()) m_monoScriptEditor.Draw(in_ts);
	}

	bool AssetBrowserPanel::draw_asset_button(
		std::string_view in_assetName,
		ImTextureID in_thumbnail,
		bool in_isSelected,
		float in_aspect,
		const ImVec2& in_uv0,
		const ImVec2& in_uv1
	) const
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 finalButtonSize = get_button_size();
		bool result = false;
		float iconLen = { finalButtonSize.x - style.FramePadding.x * 2 };

		ImGui::BeginGroup();

		// 버튼
		ImGui::InvisibleButton(
			fmt::format("{}", in_assetName).c_str(),
			finalButtonSize
		);

		bool isHovered = ImGui::IsItemHovered();
		bool isActive = ImGui::IsItemActive();

		ImU32 bgColor;
		if (isActive)		bgColor = ImGui::GetColorU32(ImGuiCol_ButtonActive), result = true;
		else if (isHovered) bgColor = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
		else				bgColor = in_isSelected ? ImGui::GetColorU32(ImGuiCol_ButtonActive) : ImGui::GetColorU32(ImVec4{ 0.f,0.f,0.f,0.f });

		ImVec2 pMin = ImGui::GetItemRectMin();
		ImVec2 pMax = ImGui::GetItemRectMax();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(pMin, pMax, bgColor, style.FrameRounding);
		drawList->AddRect(pMin, pMax, ImGui::GetColorU32(ImGuiCol_Border), style.FrameRounding); // BORDER

		float regionW = pMax.x - pMin.x;
		float regionH = pMax.y - pMin.y;
		float offsetX = (regionW - iconLen) * 0.5f;
		float offsetY = (regionH - iconLen) * 0.5f;

		float iconX = pMin.x + offsetX;
		float iconY = pMin.y + offsetY;

		// 썸네일
		ImGuiEx::AddCenterAlignImage(
			in_thumbnail,
			in_aspect,
			ImVec2{ iconX, iconY },
			ImVec2{ iconX + iconLen, iconY + iconLen },
			in_uv0, in_uv1
		);

		// 툴팁
		ImGuiEx::ItemTooltip(in_assetName);

		// 파일 이름
		std::string finalText = in_assetName.data();
		ImVec2 textSize = ImGui::CalcTextSize(in_assetName.data());
		if (textSize.x > finalButtonSize.x)
		{
			float oneTextLen = ImGui::CalcTextSize("X").x;
			finalText = finalText.substr(0, static_cast<size_t>(finalButtonSize.x / oneTextLen)) + "...";
			textSize = ImGui::CalcTextSize(finalText.c_str());
		}

		float textOffsetX = (finalButtonSize.x - textSize.x) * 0.5f;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffsetX);
		ImGui::TextUnformatted(finalText.c_str());

		ImGui::EndGroup();
		return result;
	}

	void AssetBrowserPanel::draw_mesh_browser()
	{
	}

	void AssetBrowserPanel::draw_game_texture_browser()
	{
		ImChildWindow cWin{ "AssetViewer" ,{0.f,0.f}, ImGuiChildFlags_Border };
		draw_asset_viewer<GameTexture>();
	}

	void AssetBrowserPanel::draw_flipbook_browser()
	{
		auto& gData = GetCrabEditorData();

		ImGui::SameLine();
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.plusIcon), "Create"))
		{
			m_flipbookEditorPanel.OpenPanel(nullptr, FlipbookEditorPanel::eMode::Create);
		}

		ImGui::SameLine();
		ImGui::BeginDisabled(!m_focusAssetID.IsValid());
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.pencilIcon), "Edit"))
		{
			AssetID<Flipbook> id{ m_focusAssetID.index, m_focusAssetID.token };
			const Flipbook* flipbook = TryGetAsset(id);
			if (flipbook)
			{
				m_flipbookEditorPanel.OpenPanel(flipbook, FlipbookEditorPanel::eMode::Edit);
			}
			else
			{
				assert(false);
			}
		}

		ImGui::SameLine();
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.trashIcon), "Delete"))
		{
			AssetID<Flipbook> id{ m_focusAssetID.index, m_focusAssetID.token };
			const Flipbook* flipbook = TryGetAsset(id);
			if (flipbook)
			{
				if (GetAssetManager().RemoveAsset(id))
				{
					SendVisualLog(
						eVisualLogLevel::Info,
						"Flipbook delete success"
					);

					m_focusAssetID.Clear();
				}
			}
			else
			{
				assert(false);
			}
		}
		ImGui::EndDisabled();

		ImChildWindow cWin{ "AssetViewer" ,{0.f,0.f}, ImGuiChildFlags_Border };
		draw_asset_viewer<Flipbook>();
	}

	void AssetBrowserPanel::draw_mono_script_browser()
	{
		auto& gData = GetCrabEditorData();

		ImGui::SameLine();
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.plusIcon), "Create"))
		{
			m_monoScriptEditor.OpenPanel(nullptr, MonoScriptEditor::eMode::Create);
		}

		ImGui::SameLine();
		ImGui::BeginDisabled(!m_focusAssetID.IsValid());
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.pencilIcon), "Edit"))
		{
			AssetID<MonoScript> id{ m_focusAssetID.index, m_focusAssetID.token };
			const MonoScript* mono = TryGetAsset(id);
			if (mono)
			{
				m_monoScriptEditor.OpenPanel(nullptr, MonoScriptEditor::eMode::Edit);
			}
			else
			{
				assert(false);
			}
		}

		ImGui::SameLine();
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.trashIcon), "Delete"))
		{
			AssetID<MonoScript> id{ m_focusAssetID.index, m_focusAssetID.token };
			const MonoScript* mono = TryGetAsset(id);
			if (mono)
			{
				if (GetAssetManager().RemoveAsset(id))
				{
					SendVisualLog(
						eVisualLogLevel::Info,
						"Mono script delete success."
					);

					m_focusAssetID.Clear();
				}
			}
			else
			{
				assert(false);
			}
		}
		ImGui::EndDisabled();

		ImChildWindow cWin{ "AssetViewer" ,{0.f,0.f}, ImGuiChildFlags_Border };
		draw_asset_viewer<MonoScript>();
	}

	void AssetBrowserPanel::draw_sprite_browser()
	{
		auto& gData = GetCrabEditorData();

		ImGui::SameLine();
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.plusIcon), "Create"))
		{
			m_spriteEditorPanel.OpenPanel(nullptr, SpriteEditorPanel::eMode::Create);
		}

		ImGui::SameLine();
		ImGui::BeginDisabled(!m_focusAssetID.IsValid());
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.pencilIcon), "Edit"))
		{
			AssetID<Sprite> id{ m_focusAssetID.index, m_focusAssetID.token };
			const Sprite* sprite = TryGetAsset(id);
			if (sprite)
			{
				m_spriteEditorPanel.OpenPanel(sprite, SpriteEditorPanel::eMode::Edit);
			}
			else
			{
				assert(false);
			}
		}

		ImGui::SameLine();
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.trashIcon), "Delete"))
		{
			AssetID<Sprite> id{ m_focusAssetID.index, m_focusAssetID.token };
			const Sprite* sprite = TryGetAsset(id);
			if (sprite)
			{
				if (GetAssetManager().RemoveAsset(id))
				{
					SendVisualLog(
						eVisualLogLevel::Info,
						"Sprite delete success."
						);

					m_focusAssetID.Clear();
				}
			}
			else
			{
				assert(false);
			}
		}
		ImGui::EndDisabled();

		ImChildWindow cWin{ "AssetViewer" ,{0.f,0.f}, ImGuiChildFlags_Border };
		draw_asset_viewer<Sprite>();
	}

	void AssetBrowserPanel::handle_input()
	{
		if (!ImGui::IsWindowHovered(ImGuiFocusedFlags_ChildWindows)) return;

		if (Input::IsKeyDown(eKey::LeftCtrl))
		{
			float dt = Input::GetMouseScrollDeltaY();

			if (dt != 0.f)
			{
				constexpr float SCALE_FACTOR = 1 / 10.f;
				m_browserZoomRatio += SCALE_FACTOR * dt;
				m_browserZoomRatio = std::clamp(m_browserZoomRatio, 0.5f, 1.7f);
			}
		}
	}

	ImVec2 AssetBrowserPanel::get_button_size() const
	{
		constexpr ImVec2 BUTTON_DEFAULT_SIZE = { 176, 176 };
		return BUTTON_DEFAULT_SIZE * m_browserZoomRatio;
	}

}
