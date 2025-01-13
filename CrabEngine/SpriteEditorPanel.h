#pragma once
#include "AssetPayload.h"
#include "CrabEditorData.h"
#include "PanelDrawHelper.h"

namespace crab
{
	class SpriteEditorPanel
	{
	public:
		void SetEditData(const Sprite& in_sprite)
		{
			m_editSprite = in_sprite;
		}

		void Draw(bool* p_open = nullptr)
		{
			ImWindow window{ "Sprite Editor" , p_open };

			// Handle input
			ImGuiContext* c = ImGui::GetCurrentContext();
			ImDrawList* draw = ImGui::GetWindowDrawList();

			// Edit Region
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
					ImVec2{ImGui::GetContentRegionAvail().x * 0.7f, 0.f },
					ImGuiChildFlags_Border
				};

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

			ImGui::SameLine();

			// Inspector Region
			ImGui::BeginGroup();
			{
				ImGui::AlignTextToFramePadding();
				ImGui::TextUnformatted("Preview");
				const GameTexture* texture = TryGetAsset(m_editSprite.textureID);

				{
					ImChildWindow previewWindow{
						"previewWindow",
						ImVec2{0.f,  ImGui::GetContentRegionAvail().y * 0.4f },
						ImGuiChildFlags_Border
					};

					if (texture)
					{
						const ImVec2 p1 = ImGui::GetCursorPos();
						const ImVec2 p2 = p1 + ImGui::GetContentRegionAvail();

						auto [width, height] = texture->texture->GetSize();

						const float normWidth = m_editSprite.desc.width / width;
						const float normHeight = m_editSprite.desc.height / height;
						const ImVec2 uv0 = { normWidth * m_editSprite.desc.pivotCol, normWidth * m_editSprite.desc.pivotRow };

						draw->PushClipRect(p1, p2);
						ImGuiEx::AddCenterAlignImage(
							ToImTextureID(texture->texture),
							texture->texture->GetAspect(),
							p1, p2, uv0, uv0 + ImVec2{ normWidth, normHeight }
						);
						draw->PopClipRect();
					}
				}

				{
					ImChildWindow inspectorWindow{
						"inspectorWindow",
						ImVec2{ 0.f,  0.f },
						ImGuiChildFlags_Border
					};

					if (texture)
					{
						auto [width, height] = texture->texture->GetSize();
						if (ImGui::CollapsingHeader("Texture Inspector", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGuiEx::GuiWithLabel("Texture Name", [&]() {ImGui::TextUnformatted(texture->name.c_str()); });
							ImGuiEx::GuiWithLabel("Width", [&]() {ImGui::Text("%d", width); });
							ImGuiEx::GuiWithLabel("Height", [&]() {ImGui::Text("%d", height); });
						}

						if (ImGui::CollapsingHeader("Sprite Inspector", ImGuiTreeNodeFlags_DefaultOpen))
						{
							ImGuiEx::GuiWithLabel("Sprite Name", [&]()
								{
									static char s_buffer[100] = {};
									if (ImGui::InputText("##SpEdtInput", s_buffer, ARRAYSIZE(s_buffer)))
									{
										m_editSprite.name = s_buffer;
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
			}
			ImGui::EndGroup();
		}

	private:
		Sprite	m_editSprite = {};
		float	m_zoomRatio = 1.f;
		ImVec2	m_offset = { 0.f,0.f };
	};
}
