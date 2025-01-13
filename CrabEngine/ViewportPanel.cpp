#include "CrabEnginePch.h"
#include "ViewportPanel.h"
#include "IRenderTarget.h"
#include "CrabEditorData.h"
#include "EditorEvent.h"
#include "IUnorderedAccessView.h"
#include "PanelDrawHelper.h"

namespace crab
{
	ViewportPanel::ViewportPanel()
	{
		auto& gData = GetCrabEditorData();
		m_editRenderTarget = gData.editRenderFrame->GetRenderTarget(0);
		m_pickingView = gData.editRenderFrame->GetUnorderedResourceView(0);
	}

	void ViewportPanel::OnImGuiRender(TimeStamp& in_ts)
	{
		auto& gData = GetCrabEditorData();
		ImWindow window("Viewport", nullptr, ImGuiWindowFlags_NoNavInputs);
		ImGuiStyle& style = ImGui::GetStyle();

		_handle_input_(in_ts);

#pragma region Viewport Button
		// Viewport Buttons
		{
			ImGui::SetCursorScreenPos(ImGui::GetCursorScreenPos() - style.FramePadding);
			ImChildWindow child{ "child" , { ImGui::GetWindowWidth() + style.FramePadding.x, 40.f } , NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse };

			ImColorStyle c0{ ImGuiCol_Button, ImVec4{0.f,0.f,0.f,0.f} };
			ImColorStyle c1{ ImGuiCol_ButtonHovered, ImVec4{1.f,1.f,1.f,0.3f} };
			ImColorStyle c2{ ImGuiCol_ButtonActive, ImVec4{1.f,1.f,1.f,0.15f} };

			float btnsWidth = m_buttonSize.x + 2 * style.FramePadding.x;

			switch (gData.editorState)
			{
			case eEditorState::Runtime:
			{
				ImGui::SetCursorPosX(0.5f * (ImGui::GetContentRegionAvail().x - btnsWidth));
				_draw_reset_button_();
				break;
			}

			case eEditorState::Edit:
			{
				ImGui::SetCursorPosX(0.5f * ImGui::GetContentRegionAvail().x - btnsWidth);
				_draw_play_button_();
				ImGui::SameLine();
				if (_draw_simul_button_())
				{
					Scene* scene = GetSceneManager().TryGetCurrentScene();
					if (scene)
					{
						gData.tempSceneData.ToJson(scene);
						GetSceneManager().RestartScene();
					}
				}

				break;
			}

			case eEditorState::SimulatePlay:
			{
				ImGui::SetCursorPosX(0.5f * ImGui::GetContentRegionAvail().x - btnsWidth);
				_draw_reset_button_();
				ImGui::SameLine();
				_draw_pause_button_();
				break;
			}

			case eEditorState::SimulateStop:
			{
				ImGui::SetCursorPosX(0.5f * ImGui::GetContentRegionAvail().x - 1.5f * btnsWidth);
				_draw_reset_button_();
				ImGui::SameLine();
				_draw_simul_button_();
				ImGui::SameLine();
				_draw_next_button_(in_ts);
				break;
			}

			default: assert(false);
			}
		}
#pragma endregion Viewport Button

#pragma region GameScene Render
		// Scene
		ImVec2 cursor = ImGui::GetCursorScreenPos() - style.ItemSpacing;
		ImTextureID	id = reinterpret_cast<ImTextureID>(m_editRenderTarget->GetTexture());
		ImGuiEx::AddCenterAlignImage(
			id,
			m_editRenderTarget->GetAspect(),
			cursor,
			cursor + ImGui::GetContentRegionAvail() + style.ItemSpacing * 2.f,
			{ 0.f,0.f }, { 1.f,1.f },
			&m_viewportScreenPos,
			&m_viewportSize
		);

#pragma endregion GameScene Render

#pragma region ImGuizmo

		// ImGuizmo
		if (gData.editorState == eEditorState::Edit && gData.selectedEntity.IsValid())
		{
			ImGuizmo::SetOrthographic(gData.editorCamera.mode == eProjection::Orthographic);
			ImGuizmo::BeginFrame();
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_viewportScreenPos.x, m_viewportScreenPos.y, m_viewportSize.x, m_viewportSize.y);

			Mat cmrView = gData.editorCamera.GetView();
			Mat cmrProj = gData.editorCamera.GetProj(m_editRenderTarget->GetAspect());
			Mat entityWorld = gData.selectedEntity.FindComponent<Transform>().GetWorld();

			ImGuizmo::Manipulate(
				reinterpret_cast<float*>(&cmrView),
				reinterpret_cast<float*>(&cmrProj),
				gData.guizmoOp,
				m_guizmoMode,
				reinterpret_cast<float*>(&entityWorld)
			);

			if (ImGuizmo::IsUsing())
			{
				Transform& trans = gData.selectedEntity.FindComponent<Transform>();
				Quat quat = {};
				entityWorld.Decompose(trans.scaling, quat, trans.position);
				trans.rotation = quat.ToEuler();
			}
		}
#pragma endregion

#pragma region Overlay

		// Overlay
		{
			ImColorStyle c0{ ImGuiCol_Button, ImVec4{0.f,0.f,0.f,0.5f} };
			ImColorStyle c1{ ImGuiCol_ButtonHovered, ImVec4{0.f,0.f,0.f,0.7f} };
			ImColorStyle c2{ ImGuiCol_ButtonActive, ImVec4{0.f,0.f,0.f,0.7f} };
			ImStyle s0{ ImGuiStyleVar_FrameRounding, 5.f };
			ImGuiStyle& style = ImGui::GetStyle();

			// 플레이 모드 오버레이
			{
				ImGui::SetCursorScreenPos(cursor + style.WindowPadding);
				ImGui::Button(fmt::format("Play mode: {}", ToString(gData.editorState)).c_str());

				if (ImPopupItem item{})
				{
					static auto SwitchModeFunc = [&](eEditorState in_editorState)
						{
							if (ImGui::MenuItem(ToString(in_editorState), nullptr, in_editorState == gData.editorState))
							{
								SetEditorState_EditorEvent e;
								e.m_editorState = in_editorState;
								GetCrabEngine().DispatchEvent(e);
							}
						};

					SwitchModeFunc(eEditorState::Edit);
					SwitchModeFunc(eEditorState::Runtime);
					SwitchModeFunc(eEditorState::SimulatePlay);
					SwitchModeFunc(eEditorState::SimulateStop);
				}
			}

			// Guizmo
			{
				ImGui::SameLine();
				{
					ImTextureID id = 0;
					std::string tooltipString;
					switch (gData.guizmoOp)
					{
					case ImGuizmo::TRANSLATE:
						id = reinterpret_cast<ImTextureID>(gData.translateIcon->GetNativeTexture());
						break;

					case ImGuizmo::ROTATE:
						id = reinterpret_cast<ImTextureID>(gData.rotateIcon->GetNativeTexture());
						break;

					case ImGuizmo::SCALE:
						id = reinterpret_cast<ImTextureID>(gData.scaleIcon->GetNativeTexture());
						break;

					default: assert(false);
					}

					ImGuiContext* c = ImGui::GetCurrentContext();
					ImGui::ImageButton("imgzmBnt", id, { c->FontSize,c->FontSize });
				}

				// Draw Gizmo Inspector
				if (ImPopupItem item{ "", ImGuiPopupFlags_MouseButtonLeft })
				{
					ImGui::TextUnformatted("Gizmo Operator");
					if (ImGui::RadioButton("Translate", gData.guizmoOp == ImGuizmo::TRANSLATE)) gData.guizmoOp = ImGuizmo::TRANSLATE;
					ImGuiEx::ItemTooltip("Hotkey is (Q)");

					if (ImGui::RadioButton("Rotation", gData.guizmoOp == ImGuizmo::ROTATE)) gData.guizmoOp = ImGuizmo::ROTATE;
					ImGuiEx::ItemTooltip("Hotkey is (W)");

					if (ImGui::RadioButton("Scaling", gData.guizmoOp == ImGuizmo::SCALE)) gData.guizmoOp = ImGuizmo::SCALE;
					ImGuiEx::ItemTooltip("Hotkey is (E)");

					ImGui::TextUnformatted("Gizmo Coordinate");
					if (ImGui::RadioButton("World", m_guizmoMode == ImGuizmo::WORLD)) m_guizmoMode = ImGuizmo::WORLD;
					if (ImGui::RadioButton("Local", m_guizmoMode == ImGuizmo::LOCAL)) m_guizmoMode = ImGuizmo::LOCAL;
				}
			}

			// 카메라 오버레이
			{
				float len = ImGui::CalcTextSize("X").y;

				ImGui::SameLine(ImGui::GetContentRegionAvail().x - len - style.WindowPadding.x - 2 * style.FramePadding.x);
				if (ImGui::ImageButton("cameraBtn", (ImTextureID)gData.cameraIcon->GetNativeTexture(), { len, len }))
				{
					m_drawEditCmrInspector = !m_drawEditCmrInspector;
				}
			}
		}

		// camera inspector
		if (m_drawEditCmrInspector)
		{
			ImGui::SetNextWindowPos(ImGui::GetMousePos(), ImGuiCond_Appearing);
			ImGui::SetNextWindowSize({ 500.f, 0.f }, ImGuiCond_Always);
			ImWindow window{ "Editor Camera Inspector", &m_drawEditCmrInspector };

			// Camera
			{
				ImGui::SeparatorText("Projection Mode");
				static const char* s_projectionString[] = { "Orthographic", "Perspective" };
				ImGui::RadioButton(s_projectionString[0], reinterpret_cast<int*>(&gData.editorCamera.mode), 0);
				ImGui::SameLine();
				ImGui::RadioButton(s_projectionString[1], reinterpret_cast<int*>(&gData.editorCamera.mode), 1);

				ImGui::SeparatorText("Camera Property");

				ImGuiEx::GuiWithLabel("FOV", [&]()
					{
						ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
						ImGui::DragFloat("##EdtCmrFov", &gData.editorCamera.fov,
							0.005f, 0.01f, 179.9f,
							fmt::format("{:.2f}", crab::math::RadToDeg(gData.editorCamera.fov)).c_str()
						);
					}
				);
			}
			ImGui::SeparatorText("Transform");
			ImGuiEx::TransformInspector("EdtCmr", gData.editorCamera.transform);
		}

#pragma endregion Overlay

	}

	void ViewportPanel::_handle_input_(TimeStamp& in_ts)
	{
		if (!ImGui::IsWindowHovered()) return;
		auto& gData = GetCrabEditorData();

		if (gData.editorState == eEditorState::Edit)
		{
			constexpr float MOVE_SCALE = 1 / 1000.f;
			constexpr float SCROLL_SCALE = 1 / 50.f;

			// Camera move x, y, angle
			if (Input::IsMouseDown(eMouse::Left))
			{
				auto [dx, dy] = Input::GetMousePosDelta();

				if (Input::IsKeyDown(eKey::Space))
				{
					gData.editorCamera.transform.position += MOVE_SCALE * dx * -gData.editorCamera.transform.RightVector();
					gData.editorCamera.transform.position += MOVE_SCALE * dy * gData.editorCamera.transform.UpVector();
				}
				else if (Input::IsKeyDown(eKey::LeftCtrl))
				{
					gData.editorCamera.transform.rotation.x -= dy * MOVE_SCALE;
					gData.editorCamera.transform.rotation.y -= dx * MOVE_SCALE;
				}
			}

			// Camera move z
			float dtScroll = Input::GetMouseScrollDeltaY();
			if (dtScroll != 0.f)
			{
				gData.editorCamera.transform.position += SCROLL_SCALE * dtScroll * gData.editorCamera.transform.ForwardVector();
			}
		}

		// entity picking
		if (Input::IsMousePress(eMouse::Left) && !ImGuizmo::IsOver())
		{
			auto [x, y] = Input::GetMousePos();
			Vec2 mousePosInViewport = { x - m_viewportScreenPos.x, y - m_viewportScreenPos.y };

			auto [rtX, rtY] = m_pickingView->GetSize();
			uint32 mousePosInRenderTargetX = rtX * (mousePosInViewport.x / m_viewportSize.x);
			uint32 mousePosInRenderTargetY = rtY * (mousePosInViewport.y / m_viewportSize.y);
			uint32 entity = UINT32_LIMIT;
			m_pickingView->GetPixel(mousePosInRenderTargetX, mousePosInRenderTargetY, &entity);

			if (entity == UINT32_LIMIT)
			{
				gData.selectedEntity = Entity::s_null;
			}
			else
			{
				gData.selectedEntity = GetSceneManager().TryGetCurrentScene()->TryFindByID(entity);
			}
		}
	}

	void ViewportPanel::_draw_play_button_() const
	{
		auto& gData = GetCrabEditorData();
		if (ImGui::ImageButton("Play", ToImTextureID(gData.playIcon), m_buttonSize))
		{
			SetEditorState_EditorEvent e;
			e.m_editorState = eEditorState::Runtime;
			GetCrabEngine().DispatchEvent(e);
		}

		ImGuiEx::ItemTooltip("Running the game in runtime mode.");
	}

	bool ViewportPanel::_draw_simul_button_() const
	{
		auto& gData = GetCrabEditorData();
		if (ImGui::ImageButton("Simulate", ToImTextureID(gData.simulateIcon), m_buttonSize))
		{
			SetEditorState_EditorEvent e;
			e.m_editorState = eEditorState::SimulatePlay;
			GetCrabEngine().DispatchEvent(e);
			return true;
		}

		ImGuiEx::ItemTooltip("Running the game in simulation mode.");
		return false;
	}

	void ViewportPanel::_draw_pause_button_() const
	{
		auto& gData = GetCrabEditorData();
		if (ImGui::ImageButton("Pause", ToImTextureID(gData.pauseIcon), m_buttonSize))
		{
			SetEditorState_EditorEvent e;
			e.m_editorState = eEditorState::SimulateStop;
			GetCrabEngine().DispatchEvent(e);
		}

		ImGuiEx::ItemTooltip("Pause frame.");
	}

	void ViewportPanel::_draw_reset_button_() const
	{
		auto& gData = GetCrabEditorData();
		if (ImGui::ImageButton("Reset", ToImTextureID(gData.resetIcon), m_buttonSize))
		{
			SetEditorState_EditorEvent e;
			e.m_editorState = eEditorState::Edit;
			GetCrabEngine().DispatchEvent(e);
		}

		ImGuiEx::ItemTooltip("Back to edit mode.");
	}

	void ViewportPanel::_draw_next_button_(const TimeStamp& in_ts) const
	{
		auto& gData = GetCrabEditorData();
		if (ImGui::ImageButton("Next", ToImTextureID(gData.nextFrameIcon), m_buttonSize))
		{
			NextFrame_EditorEvent e;
			e.m_timeStamp = in_ts;
			GetCrabEngine().DispatchEvent(e);
		}

		ImGuiEx::ItemTooltip("Play to next frame.");
	}
}
