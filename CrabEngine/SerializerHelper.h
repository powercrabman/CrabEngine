#pragma once

#define SERIALIZERALBE_INTRUSIVE(TypeName, ...)		NLOHMANN_DEFINE_TYPE_INTRUSIVE(TypeName, __VA_ARGS__)
#define SERIALIZERABLE(TypeName, ...)				NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TypeName, __VA_ARGS__)
#define ENUM_SERIALIZERABLE(EnumType, ...)			NLOHMANN_JSON_SERIALIZE_ENUM(EnumType, __VA_ARGS__)
#define ADL_SERIALIZERABLE(Type, ...)\
namespace nlohmann\
{\
	template <>\
	struct adl_serializer<Type>\
	{\
	static void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_TO, __VA_ARGS__)) } \
	static void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(NLOHMANN_JSON_FROM, __VA_ARGS__)) }\
	};\
}
//===================================================
//                   About Math
//===================================================

ADL_SERIALIZERABLE(Vec2, x, y);
ADL_SERIALIZERABLE(Vec3, x, y, z);
ADL_SERIALIZERABLE(Vec4, x, y, z, w);
ADL_SERIALIZERABLE(Mat, 
	_11, _12, _13, _14,
	_21, _22, _23, _24,
	_31, _32, _33, _34,
	_41, _42, _43, _44
);


ADL_SERIALIZERABLE(DirectX::XMFLOAT2, x, y);
ADL_SERIALIZERABLE(DirectX::XMFLOAT3, x, y, z);
ADL_SERIALIZERABLE(DirectX::XMFLOAT4, x, y, z, w);

ADL_SERIALIZERABLE(AABB, Center, Extents);
ADL_SERIALIZERABLE(OBB, Center, Extents, Orientation);

//===================================================
//                   About ImGui
//===================================================

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ImVec4, x, y, z, w);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ImVec2, x, y);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ImGuiStyle,
	Alpha,
	DisabledAlpha,
	WindowPadding,
	WindowRounding,
	WindowBorderSize,
	WindowMinSize,
	WindowTitleAlign,
	WindowMenuButtonPosition,
	ChildRounding,
	ChildBorderSize,
	PopupRounding,
	PopupBorderSize,
	FramePadding,
	FrameRounding,
	FrameBorderSize,
	ItemSpacing,
	ItemInnerSpacing,
	CellPadding,
	TouchExtraPadding,
	IndentSpacing,
	ColumnsMinSpacing,
	ScrollbarSize,
	ScrollbarRounding,
	GrabMinSize,
	GrabRounding,
	LogSliderDeadzone,
	TabRounding,
	TabBorderSize,
	TabMinWidthForCloseButton,
	TabBarBorderSize,
	TabBarOverlineSize,
	TableAngledHeadersAngle,
	TableAngledHeadersTextAlign, ColorButtonPosition,
	ButtonTextAlign,
	SelectableTextAlign,
	SeparatorTextBorderSize,
	SeparatorTextAlign,
	SeparatorTextPadding,
	DisplayWindowPadding,
	DisplaySafeAreaPadding,
	DockingSeparatorSize,
	MouseCursorScale,
	AntiAliasedLines,
	AntiAliasedLinesUseTex,
	AntiAliasedFill,
	CurveTessellationTol,
	CircleTessellationMaxError,
	Colors,
	HoverStationaryDelay,
	HoverDelayShort,
	HoverDelayNormal,
	HoverFlagsForTooltipMouse,
	HoverFlagsForTooltipNav
);
