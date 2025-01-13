#include "CrabEnginePch.h"
#include "SceneHierarchyPanel.h"
#include "PanelDrawHelper.h"
#include "SceneSerializer.h"
#include "CrabEditorData.h"

namespace crab
{
	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		// load Scenes' names
		for (const auto& sc : GetSceneManager())
		{
			std::string name = sc.second->ToString();
			m_sceneNames.emplace_back(name);
		}
	}

	void SceneHierarchyPanel::OnImGuiRender(TimeStamp& in_ts)
	{
		auto& gData = GetCrabEditorData();
		ImGui::SetNextWindowSize(ImVec2{ 300.f, 500.f }, ImGuiCond_FirstUseEver);

		ImWindow window("Scene Hierarchy");

#pragma region SceneSelector
		{
			ImGuiContext* c = ImGui::GetCurrentContext();

			// Menu
			{
				ImStyleX s0{ ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding.y };
				ImGuiEx::IconButton(ToImTextureID(gData.plusIcon));
				if (ImPopupItem item{}) { _draw_scene_menu_(); }
			}

			ImGui::SameLine();

			// SceneSelector
			Scene* curScene = GetSceneManager().TryGetCurrentScene();
			if (curScene && m_sceneNames[m_sceneNameIndex] != curScene->ToString())
			{
				uint32 newIndex = 0;
				for (std::string_view sceneName : m_sceneNames)
				{
					if (sceneName == curScene->ToString())
					{
						m_sceneNameIndex = newIndex;
						break;
					}
					++newIndex;
				}
			}

			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			if (ImGui::BeginCombo("##ScSelector", m_sceneNames[m_sceneNameIndex].c_str()))
			{
				// TODO Clipping 필요
				for (uint32 idx = 0; idx < m_sceneNames.size(); ++idx)
				{
					const bool enableBit = !std::strcmp(GetSceneManager().TryGetCurrentScene()->ToString(), m_sceneNames[idx].c_str());
					if (ImGui::Selectable(m_sceneNames[idx].c_str(), enableBit))
					{
						Scene* nextScene = GetSceneManager().TryFindSceneByName(m_sceneNames[idx]);
						GetSceneManager().ChangeSceneByName(nextScene->ToString());
						m_sceneNameIndex = idx;

						gData.editorCamera = EditorCamera{};
						gData.editorState = eEditorState::Edit;
						gData.selectedEntity = Entity::s_null;
					}
				}

				ImGui::EndCombo();
			}

			// Scene reload modal
			if (m_reloadScModal)
			{
				m_reloadScModal = false;
				ImGui::OpenPopup("Reload Scene");
			}

			if (ImModal modal{ "Reload Scene" })
			{
				ImGui::TextUnformatted("Current scene data will be overwritten.\nIs that okay?");

				eImPopupResult result = modal.DrawButton2("Yes", "No");
				if (result == eImPopupResult::Button1)
				{
					Scene* sc = GetSceneManager().TryGetCurrentScene();

					SceneSerializer serializer;
					serializer.LoadJsonFromFile(sc->GetSceneDataPath());
					serializer.LoadSceneDataFromJson(sc);
					GetSceneManager().RestartScene();

					ImGui::CloseCurrentPopup();
				}
				else if (result == eImPopupResult::Button2)
				{
					ImGui::CloseCurrentPopup();
				}
			}
		}
#pragma endregion SceneSelector

#pragma region Entity View

		{
			Scene* sc = GetSceneManager().TryGetCurrentScene();
			if (sc)
			{
				entt::registry& rg = sc->GetRegistry();
				auto view = sc->GetView<entt::entity>();

				ImGuiListClipper clipper;
				clipper.Begin(view.size());

				while (clipper.Step())
				{
					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
					{
						auto entity = *(view.begin() + i);
						Tag* tag = rg.try_get<Tag>(entity);
						std::string label = tag ? tag->name : fmt::format("Entity {}", static_cast<uint32>(entity));

						Entity e = Entity{ rg, entity };
						bool selectedBit = (gData.selectedEntity.GetEntity() == entity);

						if (ImGui::Selectable(fmt::format("{}##{}", label, e.GetID()).c_str(), selectedBit))
						{
							gData.selectedEntity = e;
						}

						// Menu
						if (ImPopupItem popup{ nullptr, ImGuiPopupFlags_MouseButtonRight })
						{
							gData.selectedEntity = e;
							ImGui::TextUnformatted(label.c_str());
							ImGui::Separator();
							if (ImGui::MenuItem("Delete")) { sc->DeleteEntity(e); }
							if (ImGui::MenuItem("Duplicate")) { sc->DuplicateEntity(e); };
						}
					}
				}

				clipper.End();
			}
		}

#pragma endregion Entity View

#pragma region Context Popup Window

		if (ImPopupWindow popup{ "", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems })
		{
			if (ImGui::MenuItem("Create EmptyEntity"))
			{
				if (Scene* sc = GetSceneManager().TryGetCurrentScene())
				{
					gData.selectedEntity = sc->CreateEntity();
				}
			}
		}
#pragma endregion

	}

	void SceneHierarchyPanel::_draw_scene_menu_()
	{
		if (ImGui::MenuItem("Save Scene Data"))
		{
			Scene* sc = GetSceneManager().TryGetCurrentScene();
			if (sc)
			{
				SceneSerializer serializer;
				serializer.ToJson(sc);
				serializer.SaveJsonToFile(sc->GetSceneDataPath());
			}
		}

		if (ImGui::MenuItem("Reload Scene Data"))
		{
			Scene* sc = GetSceneManager().TryGetCurrentScene();
			if (sc)
			{
				m_reloadScModal = true;
			}
		}

		ImGui::Separator();
		if (ImGui::MenuItem("Create EmptyEntity"))
		{
			Scene* sc = GetSceneManager().TryGetCurrentScene();
			if (sc)
			{
				sc->CreateEntity();
			}
		}
	}

}