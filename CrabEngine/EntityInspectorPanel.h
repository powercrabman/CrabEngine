#pragma once
#include "CrabEditorData.h"
#include "PanelDrawHelper.h"

namespace crab
{
	class EntityInspectorPanel
	{
	public:
		EntityInspectorPanel() = default;
		~EntityInspectorPanel() = default;

		void OnImGuiRender(TimeStamp& in_ts);

	private:
		template<typename ComponentType>
		void draw_add_component_menu(const char* in_label, Entity& in_entity);

		template<typename ComponentType>
		void draw_add_component_menu(const char* in_label, Entity& in_entity, const std::function<void(Entity&)>& in_customCreation);

		template <class ComponentType>
		void draw_component_panel(
			const char* in_label,
			Entity& in_entity,
			const std::function<void(Entity&)>& in_func,
			const Ref<ITexture>& in_image_or_null = nullptr
		);
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename ComponentType>
	void crab::EntityInspectorPanel::draw_add_component_menu(const char* in_label, Entity& in_entity)
	{
		if (ImGui::MenuItem(in_label, (const char*)0, in_entity.HasComponent<ComponentType>()))
		{
			in_entity.TryCreateComponent<ComponentType>();
		}
	}

	template<typename ComponentType>
	void crab::EntityInspectorPanel::draw_add_component_menu(const char* in_label, Entity& in_entity, const std::function<void(Entity&)>& in_customCreation)
	{
		if (ImGui::MenuItem(in_label, (const char*)0, in_entity.HasComponent<ComponentType>()))
		{
			in_customCreation(in_entity);
		}
	}

	template <typename ComponentType>
	void crab::EntityInspectorPanel::draw_component_panel(const char* in_label, Entity& in_entity, const std::function<void(Entity&)>& in_func, const Ref<ITexture>& in_image_or_null /*= nullptr */)
	{
		if (!in_entity.HasComponent<ComponentType>()) return;

		ImGuiContext* context = ImGui::GetCurrentContext();
		ImGui::SetNextItemOpen(true, ImGuiCond_Appearing);

		ImGui::PushStyleVarX(ImGuiStyleVar_FramePadding, 5.f);
		bool isOpen = ImGui::CollapsingHeader(in_label, ImGuiTreeNodeFlags_AllowItemOverlap);
		ImGui::PopStyleVar();

		// 아이콘
		if (in_image_or_null)
		{
			ImGui::SameLine();
			ImGuiEx::IconRect((ImTextureID)in_image_or_null->GetNativeTexture());
		}

		// 설정
		{
			ImStyleX s0(ImGuiStyleVar_FramePadding, context->Style.FramePadding.y);
			ImColorStyle buttonBgStyle{ ImGuiCol_Button, ImVec4{0.f, 0.f, 0.f, 0.f} };

			ImGui::SameLine(ImGui::GetContentRegionAvail().x - context->FontSize);

			ImGui::ImageButton(
				fmt::format("CmpBtn{}", in_label).c_str(),
				ToImTextureID(GetCrabEditorData().gearIcon),
				ImVec2(context->FontSize, context->FontSize)
			);
		}

		// 팝업 메뉴
		if (ImPopupItem popup{ fmt::format("##{}", in_label).c_str(), ImGuiPopupFlags_MouseButtonLeft })
		{
			// Transform 은 삭제할 수 없는 컴포넌트
			if constexpr (!std::is_same_v<ComponentType, Transform>)
			{
				if (ImGui::MenuItem("Remove Component"))
				{
					in_entity.RemoveComponent<ComponentType>();
					return;
				}

			}
		}

		if (isOpen)
		{
			in_func(in_entity);
		}
	}

}


