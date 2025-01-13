#pragma once

inline ImTextureID ToImTextureID(const crab::Ref<crab::ITexture>& in_texture)
{
	return reinterpret_cast<ImTextureID>(in_texture->GetNativeTexture());
}

inline ImVec2 ToImGuiSize(const crab::Ref<crab::ITexture>& in_texture)
{
	auto [width, height] = in_texture->GetSize();
	return { width, height };
}