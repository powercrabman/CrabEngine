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

		_handle_input_();

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
			ImGuiEx::IconTextButton(ToImTextureID(gData.refreshIcon), "Refresh");

			// 에셋 뷰어 출력
			switch (m_focusAssetType)
			{
			case eAssetType::Mesh:
				_draw_mesh_browser_();
				break;

			case eAssetType::Flipbook:
				_draw_flipbook_browser_();
				break;

			case eAssetType::Sprite:
				_draw_sprite_browser_();
				break;

			case eAssetType::MonoScript:
				_draw_mono_script_browser_();
				break;

			case eAssetType::GameTexture:
				_draw_game_texture_browser_();
				break;

			default: assert(false);
			}
		}
		ImGui::EndGroup();

		if (m_enableSpriteEditor) m_spriteEditorPanel.Draw(&m_enableSpriteEditor);
	}

	bool AssetBrowserPanel::_draw_asset_button_(
		std::string_view in_assetName,
		ImTextureID in_thumbnail,
		bool in_isSelected,
		float in_aspect,
		const ImVec2& in_uv0,
		const ImVec2& in_uv1
	) const
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec2 finalButtonSize = _get_button_size();
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

	void AssetBrowserPanel::_draw_mesh_browser_()
	{
	}

	void AssetBrowserPanel::_draw_game_texture_browser_()
	{
		ImChildWindow cWin{ "AssetViewer" ,{0.f,0.f}, ImGuiChildFlags_Border };
		_draw_asset_viewer_<GameTexture>();
	}

	void AssetBrowserPanel::_draw_flipbook_browser_()
	{
	}


	void AssetBrowserPanel::_draw_mono_script_browser_()
	{
	}

	void AssetBrowserPanel::_draw_sprite_browser_()
	{
		auto& gData = GetCrabEditorData();

		ImGui::SameLine();
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.plusIcon), "Create"))
		{
			Sprite editData;
			editData.textureID = NullAsset<GameTexture>;
			editData.desc = {};
			m_spriteEditorPanel.SetEditData(editData);
			m_enableSpriteEditor = true;
		}

		ImGui::SameLine();
		ImGui::BeginDisabled(!m_focusAssetID.IsValid());
		if (ImGuiEx::IconTextButton(ToImTextureID(gData.pencilIcon), "Edit"))
		{
			AssetID<Sprite> id{ m_focusAssetID.index, m_focusAssetID.token };
			const Sprite* sprite = TryGetAsset(id);
			if (sprite)
			{
				Sprite edtData;
				edtData.textureID = sprite->textureID;
				edtData.desc = sprite->desc;
				m_spriteEditorPanel.SetEditData(edtData);
				m_enableSpriteEditor = true;
			}
			else
			{
				assert(false);
			}
		}
		ImGui::EndDisabled();

		ImChildWindow cWin{ "AssetViewer" ,{0.f,0.f}, ImGuiChildFlags_Border };
		_draw_asset_viewer_<Sprite>();
	}

	void AssetBrowserPanel::_handle_input_()
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

	ImVec2 AssetBrowserPanel::_get_button_size() const
	{
		constexpr ImVec2 BUTTON_DEFAULT_SIZE = { 176, 176 };
		return BUTTON_DEFAULT_SIZE * m_browserZoomRatio;
	}

}
