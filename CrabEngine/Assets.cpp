#include "CrabEnginePch.h"
#include "Assets.h"

namespace crab
{
	//===================================================
	//			      Game Texture
	//===================================================

	namespace
	{
		Scope<GameTexture> _create_game_texture_(const std::string_view in_name, const Ref<ITexture>& in_texture)
		{
			Scope<GameTexture> gameTex = MakeScope<GameTexture>();
			gameTex->name = in_name;
			gameTex->texture = in_texture;
			gameTex->type = eAssetType::GameTexture;

			return gameTex;
		}
	}

	AssetID<GameTexture> GameTexture::Create(const std::string_view in_name, const std::filesystem::path& in_path)
	{
		return Create(in_name, ITexture::Create(in_path));
	}

	AssetID<GameTexture> GameTexture::Create(const std::string_view in_name, const Ref<ITexture>& in_texture)
	{
		return GetAssetManager().EmplaceAsset(_create_game_texture_(in_name, in_texture));
	}

	AssetID<GameTexture> GameTexture::ReplaceOrCreate(const std::string_view in_name, const std::filesystem::path& in_path)
	{
		return ReplaceOrCreate(in_name, ITexture::Create(in_path));
	}

	AssetID<GameTexture> GameTexture::ReplaceOrCreate(const std::string_view in_name, const Ref<ITexture>& in_texture)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(_create_game_texture_(in_name, in_texture));
	}

	//===================================================
	//                    Flipbook
	//===================================================

	namespace
	{
		Scope<Flipbook> _create_flipbook_(const std::string_view in_name, AssetID<GameTexture> in_texID, const FlipbookDescription& in_desc)
		{
			Scope<Flipbook> flipbook = MakeScope<Flipbook>();
			flipbook->name = in_name;
			flipbook->desc = in_desc;
			flipbook->textureID = in_texID;
			flipbook->type = eAssetType::Flipbook;
			return flipbook;
		}
	}

	AssetID<Flipbook> Flipbook::Create(const std::string_view in_name, AssetID<GameTexture> in_texID, const FlipbookDescription& in_desc)
	{
		return GetAssetManager().EmplaceAsset(_create_flipbook_(in_name, in_texID, in_desc));
	}

	AssetID<Flipbook> Flipbook::ReplaceOrCreate(const std::string_view in_name, AssetID<GameTexture> in_texID, const FlipbookDescription& in_desc)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(_create_flipbook_(in_name, in_texID, in_desc));
	}

	//===================================================
	//                      Sprite
	//===================================================

	namespace
	{
		Scope<Sprite> _create_sprite_(const std::string_view in_name, AssetID<GameTexture> in_texID, const SpriteDescription& in_desc)
		{
			Scope<Sprite> sprite = MakeScope<Sprite>();
			sprite->name = in_name;
			sprite->textureID = in_texID;
			sprite->desc = in_desc;
			sprite->type = eAssetType::Sprite;

			// Create uvArray
			const GameTexture* gameTex = TryGetAsset(in_texID);
			if (!gameTex) assert(false);

			const auto [texWidth, texHeight] = gameTex->texture->GetSize();
			sprite->desc.width = in_desc.width ? in_desc.width : texWidth;
			sprite->desc.height = in_desc.height ? in_desc.height : texHeight;

			return sprite;
		}
	}

	AssetID<Sprite> Sprite::Create(const std::string_view in_name, AssetID<GameTexture> in_texID, const SpriteDescription& in_prop)
	{
		return GetAssetManager().EmplaceAsset(_create_sprite_(in_name, in_texID, in_prop));
	}

	AssetID<Sprite> Sprite::ReplaceOrCreate(const std::string_view in_name, AssetID<GameTexture> in_texID, const SpriteDescription& in_prop)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(_create_sprite_(in_name, in_texID, in_prop));
	}

	//===================================================
	//                      Mesh
	//===================================================

	namespace
	{
		Scope<Mesh> _create_mesh_(const std::string_view in_name, const Ref<Geometry>& in_geometry)
		{
			Scope<Mesh> mesh = MakeScope<Mesh>();
			mesh->name = in_name;
			mesh->geometry = in_geometry;
			mesh->type = eAssetType::Mesh;

			return mesh;
		}
	}

	AssetID<Mesh> Mesh::Create(const std::string_view in_name, const Ref<Geometry>& in_geometry)
	{
		return GetAssetManager().EmplaceAsset(_create_mesh_(in_name, in_geometry));
	}

	crab::AssetID<crab::Mesh> Mesh::ReplaceOrCreate(const std::string_view in_name, const Ref<Geometry>& in_geometry)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(_create_mesh_(in_name, in_geometry));
	}

	//===================================================
	//			         Mono Script
	//===================================================

	namespace
	{
		Scope<MonoScript> _create_mono_script_(const std::string_view in_name, std::string_view in_namespace, const std::string_view in_className)
		{
			Scope<MonoScript> script = MakeScope<MonoScript>();
			script->name = in_name;
			script->namespaceName = in_namespace;
			script->className = in_className;
			script->type = eAssetType::MonoScript;

			return script;
		}
	}

	AssetID<MonoScript> MonoScript::Create(const std::string_view in_name, std::string_view in_namespace, const std::string_view in_className)
	{
		return GetAssetManager().EmplaceAsset(_create_mono_script_(in_name, in_namespace, in_className));
	}

	AssetID<MonoScript> MonoScript::ReplaceOrCreate(const std::string_view in_name, std::string_view in_namespace, const std::string_view in_className)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(_create_mono_script_(in_name, in_namespace, in_className));
	}
}

