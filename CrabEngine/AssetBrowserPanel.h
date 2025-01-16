#pragma once
#include "AssetPayload.h"
#include "CrabEditorData.h"
#include "PanelDrawHelper.h"
#include "SpriteEditorPanel.h"
#include "FlipbookEditorPanel.h"

namespace crab
{
	class AssetBrowserPanel
	{
	public:
		 AssetBrowserPanel();
		~AssetBrowserPanel() = default;

		void OnImGuiRender(TimeStamp& in_ts);

	private:
		void	handle_input();

		template <typename AssetType>
		void	draw_asset_viewer();
		bool	draw_asset_button(
			std::string_view in_assetName,
			ImTextureID in_thumbnail = (ImTextureID)0,
			bool in_isSelected = false,
			float in_aspect = 1.f,
			const ImVec2& in_uv0 = { 0.f, 0.f }, const ImVec2& in_uv1 = { 1.f, 1.f }
		) const;

		void	draw_mesh_browser();
		void	draw_game_texture_browser();
		void	draw_flipbook_browser();
		void	draw_sprite_browser();
		void	draw_mono_script_browser();
		ImVec2	get_button_size() const;

		std::array<Ref<ITexture>, eAssetTypeCount> m_iconList;
		float		m_browserZoomRatio = 1.f;
		eAssetType	m_focusAssetType = static_cast<eAssetType>(0);
		AnyAssetID	m_focusAssetID = { UINT32_LIMIT, UINT32_LIMIT };

		SpriteEditorPanel	m_spriteEditorPanel;
		FlipbookEditorPanel m_flipbookEditorPanel;
	};

	///////////////////////////////////////////////////////////////////////////////////////////

	template <typename AssetType>
	void crab::AssetBrowserPanel::draw_asset_viewer()
	{
		ImGuiContext* c = ImGui::GetCurrentContext();
		ImTextureID texID = NULL;
		auto begin = GetAssetManager().GetBeginIterator<AssetType>();
		auto end = GetAssetManager().GetEndIterator<AssetType>();

		if constexpr (!(IS_SAME_TYPE(AssetType, Flipbook) || IS_SAME_TYPE(AssetType, Sprite) || IS_SAME_TYPE(AssetType, GameTexture)))
		{
			const Ref<ITexture>& tex = m_iconList[static_cast<uint32>(m_focusAssetType)];
			assert(tex);
			texID = ToImTextureID(tex);
		}

		float availableWidth = ImGui::GetContentRegionAvail().x;
		float buttonWidth = get_button_size().x;
		uint32 buttonPerRow = static_cast<uint32>(availableWidth / buttonWidth);
		buttonPerRow = (buttonPerRow == 0) ? 1 : buttonPerRow;
		float buttonSpacing = buttonPerRow == 1 ? 0.f : (availableWidth - buttonPerRow * buttonWidth) / (buttonPerRow - 1);

		uint32 assetCount = GetAssetManager().GetAssetContainerSize<AssetType>();
		uint32 rowCount = (assetCount + buttonPerRow - 1) / buttonPerRow;

		float rowHeight = get_button_size().y + c->Style.ItemInnerSpacing.y + c->FontSize;

		ImGuiListClipper clipper;
		clipper.Begin(rowCount, rowHeight);
		while (clipper.Step())
		{
			for (int32 row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row)
			{
				for (uint32 col = 0; col < buttonPerRow; ++col)
				{
					uint32 assetIndex = row * buttonPerRow + col;
					if (assetIndex >= assetCount)
						break;

					AssetID<AssetType> id = *(begin + assetIndex);
					const AssetType* asset = TryGetAsset<AssetType>(id);
					if (!asset) continue;

					float aspect = 1.f;
					ImVec2 uv0{ 0, 0 };
					ImVec2 uv1{ 1, 1 };

					if constexpr (IS_SAME_TYPE(AssetType, Flipbook) || IS_SAME_TYPE(AssetType, Sprite))
					{
						const GameTexture* tex = TryGetAsset(asset->textureID);
						if (tex)
						{
							texID = ToImTextureID(tex->texture);
							aspect = asset->desc.width / static_cast<float>(asset->desc.height);
							auto [width, height] = tex->texture->GetSize();
							ImVec2 normSize = { asset->desc.width / static_cast<float>(width), asset->desc.height / static_cast<float>(height) };
							uv0 = { normSize.x * asset->desc.pivotCol, normSize.y * asset->desc.pivotRow };
							uv1 = uv0 + normSize;
						}
					}
					else if constexpr (IS_SAME_TYPE(AssetType, GameTexture))
					{
						texID = ToImTextureID(asset->texture);
						auto [width, height] = asset->texture->GetSize();
						aspect = static_cast<float>(width) / static_cast<float>(height);
					}

					bool isSelected = m_focusAssetID.IsValid() ?
									  m_focusAssetID.IsSame(id.index, id.token) :
									  false;

					if (draw_asset_button(asset->name, texID, isSelected, aspect, uv0, uv1))
						m_focusAssetID = AnyAssetID{ id.index, id.token };

					if (col < buttonPerRow - 1)
					{
						ImGui::SameLine(0, buttonSpacing);
					}

					AssetPayloadSource(id);
				}
			}
		}

		clipper.End();
	}

}
