#include "CrabEnginePch.h"
#include "SpriteEditorPanel.h"
#include "EditorEvent.h"
#include "ImGuiRenderer.h"

namespace crab
{

	void crab::SpriteEditorPanel::Draw()
	{
		ImWindow window{ "Sprite Editor" , &m_visible };
		ImGuiRenderer::SetCurrentWindowDrawlistRenderState(eBlendState::None, eSamplerState::PointClamp, eDepthStencilState::None, eRasterizerState::None);

		// Handle input
		ImGuiContext* c = ImGui::GetCurrentContext();
		ImDrawList* draw = ImGui::GetWindowDrawList();

#pragma region Sprite Editor Region

		ImGui::BeginGroup();
		{
			const GameTexture* texture = TryGetAsset(m_editSprite.textureID);

			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Source Game Texture ");

			ImGui::SameLine();
			ImGuiEx::BorderText(texture ? texture->name.c_str() : "Drag & Drop Texture!");
			AssetPayloadTarget(m_editSprite.textureID);

			ImChildWindow textureWindow{
				"textureWindow",
				ImVec2{ ImGui::GetContentRegionAvail().x * 0.7f, 0.f },
				ImGuiChildFlags_Border
			};

			// ChildWindow Input
			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
			{
				float delta = Input::GetMouseScrollDeltaY();
				if (delta != 0.f)
				{
					m_zoomRatio += delta * 0.05f * m_zoomRatio;
					m_zoomRatio = std::clamp(m_zoomRatio, 0.1f, 4.f);
				}

				if (Input::IsKeyDown(eKey::Space) && Input::IsMouseDown(eMouse::Left))
				{
					auto [dx, dy] = Input::GetMousePosDelta();

					m_offset.x += dx / m_zoomRatio;
					m_offset.y += dy / m_zoomRatio;
				}
			}

			// Texture View
			if (texture)
			{
				ImVec2 contentSize = ImGui::GetContentRegionAvail();
				ImVec2 p1 = ImGui::GetCursorScreenPos();
				ImVec2 p2 = p1 + contentSize;
				draw->AddRectFilled(p1, p2, ImGui::ColorConvertFloat4ToU32({ 0.3f,0.3f ,0.3f ,1.f }));

				ImVec2 region = contentSize * m_zoomRatio;
				ImVec2 centerOffset = (contentSize - region) * 0.5f;
				ImVec2 p3 = ImGui::GetCursorScreenPos() + centerOffset + m_offset * m_zoomRatio;
				ImVec2 p4 = p3 + region;

				auto [pos, size] = ImGuiEx::CalcCenterAlignedImageScreenPlacement(texture->texture->GetAspect(), p3, p4);

				ImGui::PushClipRect(p1, p2, false);
				ImGuiEx::DrawCheckerBoard(pos, pos + size, 32.f);
				ImGuiEx::AddCenterAlignImage(ToImTextureID(texture->texture), texture->texture->GetAspect(), pos, pos + size);
				ImGui::PopClipRect();

				ImGui::InvisibleButton("dummy", ImGui::GetContentRegionAvail());
			}
			else
			{
				constexpr const char* TEXT = "Please select a source texture";
				ImVec2 p1 = ImGui::GetCursorScreenPos();
				ImVec2 p2 = p1 + ImGui::GetContentRegionAvail();
				ImVec2 p3 = (p1 + p2 - ImGui::CalcTextSize(TEXT)) * 0.5f;
				draw->AddText(p3, ImGui::GetColorU32(ImGuiCol_TextDisabled), TEXT);
			}
		}
		ImGui::EndGroup();

#pragma endregion

		ImGui::SameLine();

#pragma region Sprite Inspector Region

		ImGui::BeginGroup();
		{
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted("Preview");
			const GameTexture* texture = TryGetAsset(m_editSprite.textureID);

			{
				ImChildWindow previewWindow{
					"previewWindow",
					ImVec2{ 0.f,  ImGui::GetContentRegionAvail().y * 0.4f },
					ImGuiChildFlags_Border
				};

				// Sprite Preview
				if (texture)
				{
					const ImVec2 p1 = ImGui::GetCursorScreenPos();
					const ImVec2 p2 = p1 + ImGui::GetContentRegionAvail();

					auto [width, height] = texture->texture->GetSize();

					const float normWidth = m_editSprite.desc.width / width;
					const float normHeight = m_editSprite.desc.height / height;
					const ImVec2 uv0 = { normWidth * m_editSprite.desc.pivotCol, normWidth * m_editSprite.desc.pivotRow };
					float aspect = m_editSprite.desc.height ? m_editSprite.desc.width / (float)m_editSprite.desc.height : math::SMALL_NUM;

					ImGui::PushClipRect(p1, p2, false);
					ImGuiEx::AddCenterAlignImage(
						ToImTextureID(texture->texture),
						aspect,
						p1, p2, uv0, uv0 + ImVec2{ normWidth, normHeight }
					);
					ImGui::PopClipRect();
				}
			}

			{
				ImChildWindow inspectorWindow{
					"inspectorWindow",
					ImVec2{ 0.f,  ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeight() - c->Style.FramePadding.y - c->Style.ItemSpacing.y },
					ImGuiChildFlags_Border
				};

				// Texture Insepctor
				if (texture)
				{
					auto [width, height] = texture->texture->GetSize();
					if (ImGui::CollapsingHeader("Texture Inspector", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGuiEx::GuiWithLabel("Texture Name", [&]() {ImGui::TextUnformatted(texture->name.c_str()); });
						ImGuiEx::GuiWithLabel("Width", [&]() {ImGui::Text("%.0f", width); });
						ImGuiEx::GuiWithLabel("Height", [&]() {ImGui::Text("%.0f", height); });
					}

					// Sprite Inspector
					if (ImGui::CollapsingHeader("Sprite Inspector", ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGuiEx::GuiWithLabel("Sprite Name", [&]()
							{
								if (ImGui::InputText("##SpEdtInput", m_spriteNameInputBuffer, ARRAYSIZE(m_spriteNameInputBuffer)))
								{
									m_editSprite.name = m_spriteNameInputBuffer;
									m_spriteNameDirtyBit = true;
								}
							});

						ImGuiEx::GuiWithLabel("Width", [&]()
							{
								static uint32 s_step = 1;
								if (ImGui::InputScalar("##SpEdtWidth", ImGuiDataType_U32, &m_editSprite.desc.width, &s_step))
								{
									m_editSprite.desc.width = std::clamp(m_editSprite.desc.width, 0u, static_cast<uint32>(width));
								}
							});

						ImGuiEx::GuiWithLabel("Height", [&]()
							{
								static uint32 s_step = 1;
								if (ImGui::InputScalar("##SpEdtHeight", ImGuiDataType_U32, &m_editSprite.desc.height, &s_step))
								{
									m_editSprite.desc.height = std::clamp(m_editSprite.desc.height, 0u, static_cast<uint32>(height));
								}
							});

						ImGuiEx::GuiWithLabel("Pivot Column", [&]()
							{
								static uint32 s_step = 1;
								ImGui::InputScalar("##SpEdtCol", ImGuiDataType_U32, &m_editSprite.desc.pivotCol, &s_step);
							});

						ImGuiEx::GuiWithLabel("Pivot Row", [&]()
							{
								static uint32 s_step = 1;
								ImGui::InputScalar("##SpEdtRow", ImGuiDataType_U32, &m_editSprite.desc.pivotRow, &s_step);
							});
					}
				}
			}

			//button size
			float buttonWidth;
			buttonWidth = (ImGui::GetContentRegionAvail().x - c->Style.ItemSpacing.x * 2.f) * 0.33333f;

			ImGui::BeginDisabled(!IsValidAsset(m_editSprite.textureID));
			if (ImGui::Button("Save", { buttonWidth , 0.f }))
			{
				if (m_mode == eMode::Edit)
				{
					if (m_spriteNameDirtyBit)
					{
						if (!is_duplicated_sprite_name())
						{
							assert(!m_originalSpriteName.empty());
							AssetID<Sprite> id = GetAssetManager().TryFindAssetIDByName<Sprite>(m_originalSpriteName);
							GetAssetManager().TryRenameAsset(id, m_editSprite.name);
							save_and_close();
						}
					}
					else
					{
						save_and_close();
					}
				}
				else
				{
					save_as_and_close();
				}
			}

			ImGui::SameLine();
			if (ImGui::Button("Save as..", { buttonWidth , 0.f }))
			{
				save_as_and_close();
			}
			ImGui::EndDisabled();

			ImGui::SameLine();
			if (ImGui::Button("Cancel", { buttonWidth , 0.f }))
			{
				m_visible = false;
			}
		}
		ImGui::EndGroup();

#pragma endregion
	}

	void SpriteEditorPanel::OpenPanel(const Sprite* in_sprite_or_null, eMode in_mode)
	{
		if (in_sprite_or_null)
		{
			m_editSprite.name = in_sprite_or_null->name;
			m_editSprite.desc = in_sprite_or_null->desc;
			m_editSprite.textureID = in_sprite_or_null->textureID;
		}
		else
		{
			m_editSprite.name = "";
			m_editSprite.desc = {};
			m_editSprite.textureID = NullAsset<GameTexture>;
		}

		if (in_mode == eMode::Create)
		{
			m_spriteNameInputBuffer[0] = '\0';
			m_originalSpriteName = "";
		}
		else
		{
			::strcpy_s(m_spriteNameInputBuffer, in_sprite_or_null->name.c_str());
			m_originalSpriteName = in_sprite_or_null->name;
		}

		m_mode = in_mode;
		m_visible = true;
		m_spriteNameDirtyBit = false;
	}

	bool crab::SpriteEditorPanel::save_as_sprite()
	{
		if (is_duplicated_sprite_name())
		{
			return false;
		}

		return save_sprite();
	}

	bool SpriteEditorPanel::is_duplicated_sprite_name()
	{
		AssetID<Sprite> id = GetAssetManager().TryFindAssetIDByName<Sprite>(m_editSprite.name);
		if (IsValidAsset(id))
		{
			SendVisualLog(
				eVisualLogLevel::Error,
				fmt::format(R"(Sprite Save Failed. Sprite Name : "{}" is duplicate name)", m_editSprite.name)
			);
			return true;
		}
		return false;
	}

	bool SpriteEditorPanel::save_sprite()
	{
		if (m_editSprite.name.empty())
		{
			SendVisualLog(
				eVisualLogLevel::Error,
				"Empty asset name is not allowed."
			);
			return false;
		}

		Sprite::ReplaceOrCreate(m_editSprite.name, m_editSprite.textureID, m_editSprite.desc);
		SendVisualLog(
			eVisualLogLevel::Info,
			fmt::format(R"(Sprite Save Done. Sprite Name : "{}")", m_editSprite.name)
		);
		return true;
	}
}
