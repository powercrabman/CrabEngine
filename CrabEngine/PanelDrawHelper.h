#pragma once
#include "AssetPayload.h"

namespace crab::ImGuiEx
{
	inline void DrawCheckerBoard(const ImVec2& in_screenPos1, const ImVec2& in_screenPos2, float in_boxLen)
	{
		ImDrawList* draw = ImGui::GetWindowDrawList();
		const ImVec2 region = in_screenPos2 - in_screenPos1;
		const uint32 gridPerCol = (region.x + in_boxLen - 1.f) / in_boxLen;
		const uint32 gridPerRow = (region.y + in_boxLen - 1.f) / in_boxLen;

		ImVec2 curPos = {};
		bool colorFlipBit = false;

		ImU32 WHITE = ImGui::ColorConvertFloat4ToU32(ImVec4{ 1.f,1.f,1.f,1.f });
		ImU32 GRAY  = ImGui::ColorConvertFloat4ToU32(ImVec4{ 0.6f,0.6f,0.6f,1.f });

		for (int i = 0; i < gridPerRow; i++)
		{
			curPos.x = in_screenPos1.x;
			curPos.y = in_screenPos1.y + in_boxLen * static_cast<float>(i);
			colorFlipBit = i & 1 ? true : false;

			for (int j = 0; j < gridPerCol; j++)
			{
				ImVec2 nextPos = curPos + ImVec2{ in_boxLen,in_boxLen };
				nextPos.x = min(nextPos.x, in_screenPos2.x);
				nextPos.y = min(nextPos.y, in_screenPos2.y);

				draw->AddRectFilled(curPos, nextPos, colorFlipBit ? WHITE : GRAY);
				colorFlipBit = !colorFlipBit;

				curPos.x += in_boxLen;
			}
		}
	}

	template<typename Fn>
	inline void GuiWithLabel(const char* in_label, const Fn& in_fn, float in_lebelCellWidth = 0.f, float in_guiCellWidth = 0.f)
	{
		if (ImGui::BeginTable(in_label, 2))
		{
			ImGuiStyle& style = ImGui::GetStyle();

			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, in_lebelCellWidth ? in_lebelCellWidth : 130.f);
			ImGui::TableSetupColumn("Gui", ImGuiTableColumnFlags_WidthFixed, in_guiCellWidth ? in_guiCellWidth : ImGui::GetContentRegionAvail().x);

			ImGui::TableNextColumn();
			ImGui::AlignTextToFramePadding();
			ImGui::TextUnformatted(in_label);
			ImGui::TableNextColumn();

			in_fn();

			ImGui::EndTable();
		}
	}

	inline void ItemTooltip(std::string_view in_msg)
	{
		if (ImGui::BeginItemTooltip())
		{
			ImGui::TextUnformatted(in_msg.data());
			ImGui::EndTooltip();
		}
	}
	inline void QuestionTooltip(const char* in_str)
	{
		ImGui::TextDisabled("(?)");
		ItemTooltip(in_str);
	}

	inline void BorderText(const char* in_text, float in_width = 0.f)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 textSize = ImGui::CalcTextSize(in_text);

		float width = (in_width > 0.f)
			? (in_width - style.FramePadding.x * 2.f)
			: textSize.x;

		ImVec2 minpos = pos;
		ImVec2 maxpos = ImVec2(
			pos.x + width + style.FramePadding.x * 2.f,
			pos.y + textSize.y + style.FramePadding.y * 2.f
		);


		ImRect bb(minpos, maxpos);
		ImGui::ItemSize(bb.GetSize(), style.FramePadding.y);

		if (!ImGui::ItemAdd(bb, ImGui::GetID(in_text)))
		{
			return;
		}

		drawList->AddRectFilled(
			minpos,
			maxpos,
			ImGui::GetColorU32(ImGuiCol_FrameBg),
			style.FrameRounding
		);

		ImVec2 text_pos(
			pos.x + style.FramePadding.x,
			pos.y + style.FramePadding.y
		);

		drawList->AddText(text_pos, ImGui::GetColorU32(ImGuiCol_Text), in_text);
	}
	inline bool IconSelectable(const char* in_text, ImTextureID in_tex, bool is_selected = false, ImGuiSelectableFlags in_flags = NULL)
	{
		ImVec2 cursor = ImGui::GetCursorPos();

		bool result = ImGui::Selectable(fmt::format("##{}", in_text).c_str(), is_selected, in_flags);

		// 이미지
		ImGui::SetCursorPos(cursor);
		float fontH = ImGui::CalcTextSize("X").y;
		ImGui::Image(in_tex, ImVec2{ fontH, fontH });

		ImGui::SameLine();
		ImGui::SetCursorPosY(cursor.y);

		// 텍스트
		ImGui::TextUnformatted(in_text);
		return result;
	}
	inline bool IconButton(ImTextureID in_tex, const ImVec2& in_size = ImVec2(0.f, 0.f))
	{
		ImGuiContext* c = ImGui::GetCurrentContext();
		ImGuiStyle& style = c->Style;

		float iconSize = (in_size.x > 0.f) ? in_size.x : c->FontSize;
		float finalW = iconSize + style.FramePadding.x * 2.f;
		float finalH = iconSize + style.FramePadding.y * 2.f;

		ImGui::InvisibleButton("##IconButton", ImVec2(finalW, finalH));

		bool isClicked = ImGui::IsItemClicked();
		bool isHovered = ImGui::IsItemHovered();
		bool isActive = ImGui::IsItemActive();

		ImU32 bgColor;
		if (isActive)  bgColor = ImGui::GetColorU32(ImGuiCol_ButtonActive);
		else if (isHovered) bgColor = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
		else                bgColor = ImGui::GetColorU32(ImGuiCol_Button);

		ImVec2 pMin = ImGui::GetItemRectMin();
		ImVec2 pMax = ImGui::GetItemRectMax();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->AddRectFilled(pMin, pMax, bgColor, style.FrameRounding);

		float regionW = pMax.x - pMin.x;
		float regionH = pMax.y - pMin.y;
		float offsetX = (regionW - iconSize) * 0.5f;
		float offsetY = (regionH - iconSize) * 0.5f;

		float iconX = pMin.x + offsetX;
		float iconY = pMin.y + offsetY;

		drawList->AddImage(
			in_tex,
			ImVec2(iconX, iconY),
			ImVec2(iconX + iconSize, iconY + iconSize)
		);

		return isClicked;
	}
	inline void IconRect(ImTextureID in_tex, const ImVec2& in_imageSize = { 0.f, 0.f }, const ImVec4& in_bgCol = { 0.f,0.f,0.f,0.f })
	{
		ImGuiContext* c = ImGui::GetCurrentContext();
		ImDrawList* draw = ImGui::GetWindowDrawList();

		ImVec2 finalSize = in_imageSize;
		finalSize.x = finalSize.x ? finalSize.x : c->FontSize;
		finalSize.y = finalSize.y ? finalSize.y : c->FontSize;

		ImVec2 padding = ImVec2{ c->Style.FramePadding.y,c->Style.FramePadding.y };
		ImVec2 totalSize = finalSize + padding * 2.f;

		ImGui::Dummy(totalSize);

		ImVec2 minRect = ImGui::GetItemRectMin();
		ImVec2 maxRect = ImGui::GetItemRectMax();

		ImVec2 imageStart = minRect + padding;
		ImVec2 imageEnd = imageStart + finalSize;

		ImU32 col = ImGui::ColorConvertFloat4ToU32(in_bgCol);
		draw->AddRectFilled(minRect, maxRect, col, c->Style.FrameRounding);

		draw->AddImage(in_tex, imageStart, imageEnd);
	}
	inline bool IconTextButton(ImTextureID in_tex, const char* in_text, const ImVec2& in_size = ImVec2(0, 0))
	{
		ImGuiStyle& style = ImGui::GetStyle();

		ImVec2 textSize = ImGui::CalcTextSize(in_text);
		float  iconSize = textSize.y;

		float totalWidth = 2 * style.FramePadding.x + iconSize + style.ItemInnerSpacing.x + textSize.x;
		float totalHeight = 2 * style.FramePadding.y + textSize.y;

		ImVec2 finalSize(
			(in_size.x < totalWidth) ? totalWidth : in_size.x,
			(in_size.y < totalHeight) ? totalHeight : in_size.y
		);

		ImGui::InvisibleButton(fmt::format("##{}", in_text).c_str(), finalSize);

		bool isClicked = ImGui::IsItemClicked();
		bool isHovered = ImGui::IsItemHovered();
		bool isActive = ImGui::IsItemActive();

		ImU32 bgColor;
		if (isActive)		bgColor = ImGui::GetColorU32(ImGuiCol_ButtonActive);
		else if (isHovered) bgColor = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
		else				bgColor = ImGui::GetColorU32(ImGuiCol_Button);

		ImVec2 pMin = ImGui::GetItemRectMin();
		ImVec2 pMax = ImGui::GetItemRectMax();

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(pMin, pMax, bgColor, style.FrameRounding);

		float contentWidth = iconSize + style.ItemInnerSpacing.x + textSize.x;
		float contentHeight = max(iconSize, textSize.y);

		float totalW = pMax.x - pMin.x;
		float totalH = pMax.y - pMin.y;

		float offsetX = (totalW - contentWidth) * 0.5f;
		float offsetY = (totalH - contentHeight) * 0.5f;

		float iconX = pMin.x + offsetX;
		float iconY = pMin.y + offsetY;

		float textX = iconX + iconSize + style.ItemInnerSpacing.x;
		float textY = pMin.y + offsetY + (contentHeight - textSize.y) * 0.5f;

		drawList->AddImage(
			in_tex,
			ImVec2(iconX, iconY),
			ImVec2(iconX + iconSize, iconY + iconSize)
		);

		drawList->AddText(ImVec2(textX, textY), ImGui::GetColorU32(ImGuiCol_Text), in_text);

		return isClicked;
	}

	inline void DrawMeshCreateMenu(AssetID<Mesh>* in_out_meshID_or_null = nullptr)
	{
		static Ref<Geometry> sampleGeometry = nullptr;
		static bool s_openModal = false;

		if (ImPopupItem menu{ "MshSlt" })
		{
			if (ImMenu menu2D{ "Menu2D" })
			{
				//if (ImGui::MenuItem("Rectangle")) { sampleGeometry = GeometryFactory::DD::CreateRectColorTex(0.5f, 0.5f); s_openModal = true; }
				if (ImGui::MenuItem("Triangle")) {}
				if (ImGui::MenuItem("Round")) {}
			}

			if (ImMenu menu2D{ "Menu3D" })
			{
				if (ImGui::MenuItem("Cube")) {}
				if (ImGui::MenuItem("Cone")) {}
				if (ImGui::MenuItem("Sphere")) {}
				if (ImGui::MenuItem("Cylinder")) {}
			}
		}

		if (s_openModal) { ImGui::OpenPopup("Create Mesh"); s_openModal = false; }
		if (ImModal modal{ "Create Mesh" })
		{
			ImGui::TextUnformatted("Input mesh name");
			static char buffer[64];
			ImGui::InputText("##MshName", buffer, sizeof(buffer));

			auto result = modal.DrawButton2("Create", "Cancel");

			if (result == eImPopupResult::Button1)
			{
				AssetID<Mesh> meshID = Mesh::Create(buffer, sampleGeometry);
				if (in_out_meshID_or_null) { *in_out_meshID_or_null = meshID; }

				ImGui::CloseCurrentPopup();
			}
			else if (result == eImPopupResult::Button2)
			{
				ImGui::CloseCurrentPopup();
			}
		}
	}
	inline void FlipbookInspector(FlipbookRender& in_fbRender)
	{
		const Flipbook* fb = TryGetAsset<Flipbook>(in_fbRender.assetID);
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::SeparatorText("Flipbook Asset Inspector");
		ImGuiEx::GuiWithLabel("Name", [&]()
			{
				ImGuiEx::BorderText(fb ? fb->name.c_str() : "Drag & Drop Asset", ImGui::GetContentRegionAvail().x);
				ImGuiEx::ItemTooltip("Use other assets by Drag & Drop");

				// Drag Drop
				AssetPayloadTarget(in_fbRender.assetID);
			}
		);

		if (fb)
		{
			ImGuiEx::GuiWithLabel("Width", [&]() { ImGui::Text("%d", fb->desc.width); });
			ImGuiEx::GuiWithLabel("Height", [&]() { ImGui::Text("%d", fb->desc.height); });
			ImGuiEx::GuiWithLabel("Total Frame", [&]() { ImGui::Text("%d", fb->desc.totalFrame); });
			ImGuiEx::GuiWithLabel("Flip Duration", [&]() { ImGui::Text("%.2f sec", fb->desc.duration); });
			ImGuiEx::GuiWithLabel("Loop", [&]() { ImGui::TextUnformatted(fb->desc.loop ? "On" : "Off"); });

			ImGui::SeparatorText("Flipbook Render Inspector");
			ImGuiEx::GuiWithLabel("Current Frame", [&]()
				{
					ImGuiEx::BorderText(fmt::format("{} / {}", in_fbRender.currentFrame + 1, fb->desc.totalFrame).c_str());

					ImGui::SameLine();
					if (ImGui::ArrowButton("##PrvFrm", ImGuiDir_Left))
					{
						in_fbRender.currentFrame = (in_fbRender.currentFrame - 1 + fb->desc.totalFrame) % fb->desc.totalFrame;
					}

					ImGui::SameLine();
					if (ImGui::ArrowButton("##NxtFrm", ImGuiDir_Right))
					{
						in_fbRender.currentFrame = (in_fbRender.currentFrame + 1) % fb->desc.totalFrame;
					}
				}
			);

			ImGuiEx::GuiWithLabel("Playback", [&]()
				{
					ImGuiEx::BorderText(fmt::format("{:.2f}", in_fbRender.playbackSpeed).c_str());

					ImGui::SameLine();
					if (ImGui::ArrowButton("##DncPbs", ImGuiDir_Left)) in_fbRender.playbackSpeed -= 0.05f;

					ImGui::SameLine();
					if (ImGui::ArrowButton("##IncPbs", ImGuiDir_Right)) in_fbRender.playbackSpeed += 0.05f;
				}
			);

			ImGuiEx::GuiWithLabel("Finish", [&]() { ImGui::TextUnformatted(in_fbRender.isFinish ? "Finished" : "Playing"); });

			float buttonWidth = (ImGui::GetContentRegionAvail().x - style.ItemInnerSpacing.x) * 0.5f;
			if (ImGui::Button(in_fbRender.isPlay ? "Stop" : "Play", ImVec2{ buttonWidth, 0.f })) { in_fbRender.isPlay = !in_fbRender.isPlay; }

			ImGui::SameLine();
			if (ImGui::Button("Reset", ImVec2{ buttonWidth,0.f })) { in_fbRender.currentFrame = 0; }
		}
	}
	inline void SpriteInspector(SpriteRender& in_spRender)
	{
		const Sprite* sp = TryGetAsset<Sprite>(in_spRender.spriteID);
		constexpr float ITEM_WIDTH = 130.f;

		ImGui::SeparatorText("Sprite Asset Inspector");
		ImGuiEx::GuiWithLabel("Name", [&]()
			{
				ImGuiEx::BorderText(sp ? sp->name.c_str() : "Drag & Drop Asset", ImGui::GetContentRegionAvail().x);
				AssetPayloadTarget<Sprite>(in_spRender.spriteID);
				ImGuiEx::ItemTooltip("Use other assets by Drag & Drop");

				AssetPayloadTarget(in_spRender.spriteID);
			}, ITEM_WIDTH
		);

		if (sp)
		{
			ImGuiEx::GuiWithLabel("Width", [&]() { ImGui::Text("%d", sp->desc.width); }, ITEM_WIDTH);
			ImGuiEx::GuiWithLabel("Height", [&]() { ImGui::Text("%d", sp->desc.height); }, ITEM_WIDTH);
		}
	}
	//inline void MeshInspector(MeshRender& in_mshRender)
	//{
	//	const Mesh* msh = TryGetAsset<Mesh>(in_mshRender.spriteID);
	//	ImGuiContext* context = ImGui::GetCurrentContext();
	//	constexpr float ITEM_WIDTH = 130.f;
	//
	//	ImGui::SeparatorText("Sprite Asset Inspector");
	//	ImGuiEx::GuiWithLabel("Name", [&]()
	//		{
	//			float buttonLen = context->FontSize + context->Style.FramePadding.y * 2;
	//			ImGuiEx::BorderText(msh ? msh->name.c_str() : "Drag & Drop Asset", ImGui::GetContentRegionAvail().x - buttonLen - context->Style.ItemInnerSpacing.x - context->Style.WindowPadding.x);
	//			AssetPayloadTarget<Mesh>(in_mshRender.spriteID);
	//			ImGuiEx::ItemTooltip("Use other assets by Drag & Drop");
	//
	//			ImGui::SameLine();
	//			ImGui::ArrowButton("##MshSlt", ImGuiDir_Right);
	//
	//			DrawMeshCreateMenu(&in_mshRender.spriteID);
	//
	//		}, ITEM_WIDTH
	//	);
	//
	//	// TODO..
	//}
	inline void TransformInspector(const char* tag, crab::Transform& transform)
	{
		ImVec2 textSize = ImGui::CalcTextSize("X");
		ImGuiStyle& style = ImGui::GetStyle();

		static auto DrawAxis = [](const char* in_tag, const char* in_axis, const char* in_label, float* in_float, const ImVec2& in_textSize, float spacing, float in_dragItemLength, float in_initialValue, const ImVec4& in_buttonColor, bool in_isRotation)
			{
				{
					ImGuiStyle& style = ImGui::GetStyle();
					float len = in_textSize.y + style.FramePadding.y * 2.f;

					ImStyle s0{ ImGuiStyleVar_FramePadding, ImVec2{ len - in_textSize.x ,len - in_textSize.y } *0.5f };
					ImColorStyle c0{ ImGuiCol_Button, in_buttonColor };
					ImColorStyle c1{ ImGuiCol_ButtonHovered, in_buttonColor + ImVec4{ 0.15f,0.15f,0.15f,1.f } };
					ImColorStyle c2{ ImGuiCol_ButtonActive, in_buttonColor + ImVec4{ 0.3f,0.4f,0.3f,1.f } };
					if (ImGui::Button(fmt::format("{}##{}{}", in_axis, in_tag, in_label).c_str())) { *in_float = in_initialValue; };
				}

				ImGui::SameLine(0.f, spacing);
				ImGui::SetNextItemWidth(in_dragItemLength);

				if (in_isRotation)
				{
					float degree = crab::math::RadToDeg(*in_float);
					if (ImGui::DragFloat(
						fmt::format("##{}{}{}", in_tag, in_axis, in_label).c_str(),
						&degree, 0.1f, 0.f, 0.f,
						"%.2f"
					))
					{
						(*in_float) = crab::math::DegToRad(degree);
					}
				}
				else
				{
					ImGui::DragFloat(
						fmt::format("##{}{}{}", in_tag, in_axis, in_label).c_str(),
						in_float, 0.001f, 0.f, 0.f,
						"%.2f"
					);
				}
			};

		float spacing = 2.f;
		float buttonWidth = textSize.y + style.FramePadding.y * 2.f;
		float dragItemWidth = (ImGui::GetContentRegionAvail().x - (3 * buttonWidth + 2 * (spacing + style.ItemInnerSpacing.x))) / 3.f;

		static auto DrawTransformHelper = [](const char* in_tag, const char* in_label, Vec3& in_trans, const ImVec2& in_textSize, float spacing, float in_dragItemLength, float in_initialValue, bool in_isRotation = false)
			{
				constexpr ImVec4 COLOR_RED = ImVec4(0.941f, 0.286f, 0.141f, 1.0f);
				constexpr ImVec4 COLOR_GREEN = ImVec4(0.043f, 0.365f, 0.318f, 1.0f);
				constexpr ImVec4 COLOR_BLUE = ImVec4(0.004f, 0.404f, 0.643f, 1.0f);

				DrawAxis(in_tag, "X", in_label, &in_trans.x, in_textSize, spacing, in_dragItemLength, in_initialValue, COLOR_RED, in_isRotation);
				ImGui::SameLine();
				DrawAxis(in_tag, "Y", in_label, &in_trans.y, in_textSize, spacing, in_dragItemLength, in_initialValue, COLOR_GREEN, in_isRotation);
				ImGui::SameLine();
				DrawAxis(in_tag, "Z", in_label, &in_trans.z, in_textSize, spacing, in_dragItemLength, in_initialValue, COLOR_BLUE, in_isRotation);
			};

		ImGui::TextUnformatted("Position");
		DrawTransformHelper(tag, "Pos", transform.position, textSize, spacing, dragItemWidth, 0.f);

		ImGui::TextUnformatted("Rotation");
		DrawTransformHelper(tag, "Rot", transform.rotation, textSize, spacing, dragItemWidth, 0.f, true);

		ImGui::TextUnformatted("Scaling");
		DrawTransformHelper(tag, "Scl", transform.scaling, textSize, spacing, dragItemWidth, 1.f);
	}
	inline void CameraInspector(const char* in_tag, Camera& in_camera)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::SeparatorText("Projection Mode");
		static const char* s_projectionString[] = { "Orthographic", "Perspective" };
		ImGui::RadioButton(s_projectionString[0], (int*)&in_camera.mode, 0);
		ImGui::SameLine();
		ImGui::RadioButton(s_projectionString[1], (int*)&in_camera.mode, 1);

		constexpr float LABEL_WIDTH = 100.f;

		ImGui::SeparatorText("Camera Property");

		GuiWithLabel("FOV", [&]()
			{
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				auto label = fmt::format("##{}FOV", in_tag);
				ImGui::DragFloat(label.c_str(), &in_camera.fov, 0.005f, 0.01f, 179.9f, fmt::format("{:.2f}", crab::math::RadToDeg(in_camera.fov)).c_str());
			}
			, LABEL_WIDTH
		);

		GuiWithLabel("Near Plane", [&]()
			{
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				auto label = fmt::format("##{}NP", in_tag);
				ImGui::DragFloat(label.c_str(), &in_camera.nearPlane, 0.001f, 0.01f, 0.f);
				in_camera.nearPlane = min(in_camera.nearPlane, in_camera.farPlane - crab::math::SMALL_NUM);
			}
			, LABEL_WIDTH
		);

		GuiWithLabel("Far Plane", [&]()
			{
				ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
				auto label = fmt::format("##{}FP", in_tag);
				ImGui::DragFloat(label.c_str(), &in_camera.farPlane, 0.001f, 0.01f, 0.f);
				in_camera.farPlane = max(in_camera.farPlane, in_camera.nearPlane + crab::math::SMALL_NUM);
			}
			, LABEL_WIDTH
		);
	}

	inline void AddCenterAlignImage(ImTextureID in_image, float in_aspect, const ImVec2& in_screenPos1, const ImVec2& in_screenPos2, const ImVec2& in_uv0, const ImVec2& in_uv1, ImVec2* out_imageScreenPos, ImVec2* out_imageSize)
	{
		const ImVec2 size = in_screenPos2 - in_screenPos1;
		const float aspect = size.x / size.y;

		ImVec2 imageSize = {};
		if (aspect > in_aspect)
		{
			imageSize.y = size.y;
			imageSize.x = size.y * in_aspect;
		}
		else
		{
			imageSize.x = size.x;
			imageSize.y = size.x / in_aspect;
		}

		ImDrawList* draw = ImGui::GetWindowDrawList();

		ImVec2 p1 = in_screenPos1 + (size - imageSize) * 0.5f;
		draw->AddImage(
			in_image,
			p1,
			p1 + imageSize,
			in_uv0,
			in_uv1
		);

		if (out_imageScreenPos) { *out_imageScreenPos = p1; }
		if (out_imageSize) { *out_imageSize = imageSize; }
	}
	inline void AddCenterAlignImage(ImTextureID in_image, float in_aspect, const ImVec2& in_screenPos1, const ImVec2& in_screenPos2, const ImVec2& in_uv0 = { 0.f,0.f }, const ImVec2& in_uv1 = { 1.f,1.f })
	{
		AddCenterAlignImage(in_image, in_aspect, in_screenPos1, in_screenPos2, in_uv0, in_uv1, nullptr, nullptr);
	}

	// Screen Pos
	inline auto CalcCenterAlignedImageScreenPlacement(float in_aspect, const ImVec2& in_screenPos1, const ImVec2& in_screenPos2)
	{
		const ImVec2 size = in_screenPos2 - in_screenPos1;
		const float aspect = size.x / size.y;

		ImVec2 imageSize = {};
		if (aspect > in_aspect)
		{
			imageSize.y = size.y;
			imageSize.x = size.y * in_aspect;
		}
		else
		{
			imageSize.x = size.x;
			imageSize.y = size.x / in_aspect;
		}

		ImVec2 p1 = in_screenPos1 + (size - imageSize) * 0.5f;
		return std::make_pair(p1, imageSize);
	}
}