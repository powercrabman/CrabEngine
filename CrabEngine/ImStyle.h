#pragma once

namespace crab
{
	class ImStyle
	{
	public:
		ImStyle(ImGuiStyleVar in_style, float in_value) { ImGui::PushStyleVar(in_style, in_value); }
		ImStyle(ImGuiStyleVar in_style, const ImVec2& in_value) { ImGui::PushStyleVar(in_style, in_value); }
		~ImStyle() { ImGui::PopStyleVar(); }
	};

	class ImStyleX
	{
	public:
		ImStyleX(ImGuiStyleVar in_style, float in_value) { ImGui::PushStyleVarX(in_style, in_value); }
		~ImStyleX() { ImGui::PopStyleVar(); }
	};

	class ImStyleY
	{
	public:
		ImStyleY(ImGuiStyleVar in_style, float in_value) { ImGui::PushStyleVarY(in_style, in_value); }
		~ImStyleY() { ImGui::PopStyleVar(); }
	};

	class ImFontStyle
	{
	public:
		ImFontStyle(ImFont* in_font) { ImGui::PushFont(in_font); }
		~ImFontStyle() { ImGui::PopFont(); }
	};

	class ImColorStyle
	{
	public:
		 ImColorStyle(ImGuiCol in_col, const ImVec4& in_color) { ImGui::PushStyleColor(in_col, in_color); }
		~ImColorStyle() { ImGui::PopStyleColor(); }
	};
}