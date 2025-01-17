#include "CrabEnginePch.h"
#include "EntityInspectorPanel.h"

#include "CrabEditorData.h"
#include "Scene.h"
#include "entt/single_include/entt/entt.hpp"
#include "PanelDrawHelper.h"
#include "PhysicComponents.h"

namespace crab
{
	void EntityInspectorPanel::OnImGuiRender(TimeStamp& in_ts)
	{
		auto& gData = GetCrabEditorData();
		ImGui::SetNextWindowSize({ 300, 600 }, ImGuiCond_FirstUseEver);
		ImWindow window{ "Entity Inspector" };

		if (gData.selectedEntity.IsValid())
		{
			// Component Panel
			draw_component_panel<Tag>("TAG", gData.selectedEntity, [](Entity& in_entity)
				{
					Tag& tag = in_entity.FindComponent<Tag>();
					ImGuiEx::GuiWithLabel("Name", [&]()
						{
							static char buffer[64];
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							if (ImGui::InputText("##Name", buffer, ARRAYSIZE(buffer)))
							{
								tag.name = buffer;
							}
							else
							{
								::strcpy_s(buffer, tag.name.c_str());
							}
						}
					);
				},
				gData.tagIcon
			);

			draw_component_panel<Transform>("TRANSFORM", gData.selectedEntity, [](Entity& in_entity)
				{
					Transform& trans = in_entity.FindComponent<Transform>();
					ImGuiEx::TransformInspector("EntTrans", trans);
				},
				gData.transformIcon
			);

			draw_component_panel<MonoScriptRunner>("MONO SCRIPT RUNNER", gData.selectedEntity, [](Entity& in_entity)
				{
					MonoScriptRunner& script = in_entity.FindComponent<MonoScriptRunner>();
					const MonoScript* sc = TryGetAsset(script.assetID);
					std::string name = sc ? sc->name : "Drag & Drop Asset";

					ImGuiEx::GuiWithLabel("Name", [&]()
						{
							ImGuiEx::BorderText(sc ? sc->name.c_str() : "Drag & Drop Asset", ImGui::GetContentRegionAvail().x);
							ImGuiEx::ItemTooltip("Use other assets by Drag & Drop");

							// Drag Drop
							AssetPayloadTarget(script.assetID);
						}
					);

					if (sc)
					{
						ImGuiEx::GuiWithLabel("Namespace Name", [&]() {ImGuiEx::BorderText(sc->namespaceName.c_str()); });
						ImGuiEx::GuiWithLabel("Class Name", [&]() {ImGuiEx::BorderText(sc->className.c_str()); });
					}
				},
				gData.monoScriptIcon
			);

			draw_component_panel<Camera>("CAMERA", gData.selectedEntity, [](Entity& in_entity)
				{
					Camera& cmr = in_entity.FindComponent<Camera>();
					ImGuiEx::CameraInspector("EntCmr", cmr);
				},
				gData.cameraIcon
			);

			draw_component_panel<FlipbookRender>("FLIPBOOK RENDER", gData.selectedEntity, [](Entity& in_entity)
				{
					FlipbookRender& fbRender = in_entity.FindComponent<FlipbookRender>();
					ImGuiEx::FlipbookInspector(fbRender);
				},
				gData.flipbookIcon
			);

			draw_component_panel<SpriteRender>("SPRITE RENDER", gData.selectedEntity, [](Entity& in_entity)
				{
					SpriteRender& spRender = in_entity.FindComponent<SpriteRender>();
					ImGuiEx::SpriteInspector(spRender);
				},
				gData.spriteIcon
			);

			draw_component_panel<SphereRender>("SPHERE RENDER", gData.selectedEntity, [](Entity& in_entity)
				{
					SphereRender& sphereRender = in_entity.FindComponent<SphereRender>();
					ImGuiEx::GuiWithLabel("SprRadius", [&]()
						{
							ImGui::DragFloat("##radius", &sphereRender.radius, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Color", [&]()
						{
							ImGui::ColorPicker4("##SprColor", reinterpret_cast<float*>(&sphereRender.color), ImGuiColorEditFlags_NoInputs);
						}
					);
				},
				gData.meshIcon
			);

			draw_component_panel<BoxRender>("BOX RENDER", gData.selectedEntity, [](Entity& in_entity)
				{
					BoxRender& boxRender = in_entity.FindComponent<BoxRender>();
					ImGuiEx::GuiWithLabel("Width/Height", [&]()
						{
							ImGui::DragFloat2("##radius", reinterpret_cast<float*>(&boxRender.size), 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Color", [&]()
						{
							ImGui::ColorPicker4("##BoxColor", reinterpret_cast<float*>(&boxRender.color), ImGuiColorEditFlags_NoInputs);
						}
					);
				},
				gData.meshIcon
			);

			draw_component_panel<RigidBody2D>("RIGID BODY 2D", gData.selectedEntity, [](Entity& in_entity)
				{
					RigidBody2D& rb = in_entity.FindComponent<RigidBody2D>();
					ImGuiEx::GuiWithLabel("Type", [&]()
						{
							constexpr const char* ITEMS[] = { "Static", "Kinematic", "Dynamic" };
							const char* currentItem = ITEMS[(int)rb.type];
							if (ImGui::BeginCombo("##Type", currentItem))
							{
								for (int n = 0; n < IM_ARRAYSIZE(ITEMS); n++)
								{
									bool isSelected = currentItem == ITEMS[n];
									if (ImGui::Selectable(ITEMS[n], isSelected))
									{
										rb.type = (eBodyType)n;
									}
									if (isSelected)
									{
										ImGui::SetItemDefaultFocus();
									}
								}
								ImGui::EndCombo();
							}
						}
					);
					ImGuiEx::GuiWithLabel("Fix Rotation", [&]()
						{
							ImGui::Checkbox("##FixRotation", &rb.fixRotation);
						}
					);
				}
			);

			draw_component_panel<BoxCollider2D>("BOX COLLIDER 2D", gData.selectedEntity, [](Entity& in_entity)
				{
					BoxCollider2D& box = in_entity.FindComponent<BoxCollider2D>();
					ImGuiEx::GuiWithLabel("Width/Height", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							float* widthHeight[] = { &box.size.x, &box.size.y };
							ImGui::DragFloat2("##BoxWh", *widthHeight, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Friction", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##BoxFriction", &box.colliderData.friction, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Density", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##BoxDensity", &box.colliderData.density, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Restitution", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##BoxRestitution", &box.colliderData.restitution, 0.001f, 0.001f, 0.f);
						}
					);
				}
			);

			draw_component_panel<SphereCollider2D>("SPHERE COLLIDER 2D", gData.selectedEntity, [](Entity& in_entity)
				{
					SphereCollider2D& sphere = in_entity.FindComponent<SphereCollider2D>();
					ImGuiEx::GuiWithLabel("Center", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat2("##SphWh", (float*)&sphere.center, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Radius", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##BoxRadius", &sphere.radius, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Friction", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##SphFriction", &sphere.colliderData.friction, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Density", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##SphDensity", &sphere.colliderData.density, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Restitution", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##SphRestitution", &sphere.colliderData.restitution, 0.001f, 0.001f, 0.f);
						}
					);
				}
			);

			draw_component_panel<CapsuleCollider2D>("CAPSULE COLLIDER 2D", gData.selectedEntity, [](Entity& in_entity)
				{
					CapsuleCollider2D& capsule = in_entity.FindComponent<CapsuleCollider2D>();
					ImGuiEx::GuiWithLabel("Center1", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat2("##CapWh", (float*)&capsule.center1, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Center2", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat2("##CaphWh", (float*)&capsule.center2, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Radius", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##CapRadius", &capsule.radius, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Friction", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##CapFriction", &capsule.colliderData.friction, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Density", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##CapDensity", &capsule.colliderData.density, 0.001f, 0.001f, 0.f);
						}
					);
					ImGuiEx::GuiWithLabel("Restitution", [&]()
						{
							ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
							ImGui::DragFloat("##CapRestitution", &capsule.colliderData.restitution, 0.001f, 0.001f, 0.f);
						}
					);
				}
			);

			// Add Component Button
			{
				ImGui::NewLine();
				ImGuiEx::IconTextButton(ToImTextureID(gData.plusIcon), "Add Component", ImVec2{ ImGui::GetContentRegionAvail().x, 0.f });

				if (ImPopupItem popup{ "" , ImGuiPopupFlags_MouseButtonLeft })
				{
					draw_add_component_menu<Tag>("Tag", gData.selectedEntity);
					draw_add_component_menu<Transform>("Transform", gData.selectedEntity);
					draw_add_component_menu<Camera>("Camera", gData.selectedEntity);
					draw_add_component_menu<FlipbookRender>("Flipbook Render", gData.selectedEntity);
					draw_add_component_menu<SpriteRender>("Sprite Render", gData.selectedEntity);
					draw_add_component_menu<BoxRender>("Box Render", gData.selectedEntity);
					draw_add_component_menu<SphereRender>("Sphere Render", gData.selectedEntity);
					draw_add_component_menu<MonoScriptRunner>("Mono Script Runner", gData.selectedEntity);

					if (ImMenu physic2DMenu{ "Physic2D" })
					{
						draw_add_component_menu<RigidBody2D>("RigidBody", gData.selectedEntity, [](Entity& in_entity)
							{
								in_entity.CreateComponent<RigidBody2D>();
							});
						draw_add_component_menu<BoxCollider2D>("BoxCollider2D", gData.selectedEntity, [](Entity& in_entity)
							{
								if (in_entity.HasComponent<RigidBody2D>())
								{
									in_entity.CreateComponent<BoxCollider2D>();
								}
								else
								{
									Log::Error("entity has to have RigidBody2D component.");
								}
							});
						draw_add_component_menu<SphereCollider2D>("SphereCollider2D", gData.selectedEntity, [](Entity& in_entity)
							{
								if (in_entity.HasComponent<RigidBody2D>())
								{
									in_entity.CreateComponent<SphereCollider2D>();
								}
								else
								{
									Log::Error("entity has to have RigidBody2D component.");
								}
							});
						draw_add_component_menu<CapsuleCollider2D>("CapsuleCollider2D", gData.selectedEntity, [](Entity& in_entity)
							{
								if (in_entity.HasComponent<RigidBody2D>())
								{
									in_entity.CreateComponent<CapsuleCollider2D>();
								}
								else
								{
									Log::Error("entity has to have RigidBody2D component.");
								}
							});
					}
				}
			}
		}
		else
		{
			constexpr const char* TEXT1 = "Please select the entity";
			constexpr const char* TEXT2 = "you want to edit";

			ImGuiStyle& style = ImGui::GetStyle();

			ImVec2 text1Size = ImGui::CalcTextSize(TEXT1);
			ImVec2 text2Size = ImGui::CalcTextSize(TEXT2);

			float posX = (ImGui::GetContentRegionAvail().x - text1Size.x) * 0.5f;
			float posY = (ImGui::GetContentRegionAvail().y + text1Size.y) * 0.5f;

			ImGui::SetCursorPos({ posX, posY });
			ImGui::TextDisabled(TEXT1);

			ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - text2Size.x) * 0.5f);
			ImGui::TextDisabled(TEXT2);
		}
	}
}
