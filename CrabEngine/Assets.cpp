#include "CrabEnginePch.h"
#include "Assets.h"

namespace crab
{
	//===================================================
	//			      Game Texture
	//===================================================

	namespace
	{
		Scope<GameTexture> create_game_texture(const std::string_view in_name, const Ref<ITexture>& in_texture)
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
		return GetAssetManager().EmplaceAsset(create_game_texture(in_name, in_texture));
	}

	AssetID<GameTexture> GameTexture::ReplaceOrCreate(const std::string_view in_name, const std::filesystem::path& in_path)
	{
		return ReplaceOrCreate(in_name, ITexture::Create(in_path));
	}

	AssetID<GameTexture> GameTexture::ReplaceOrCreate(const std::string_view in_name, const Ref<ITexture>& in_texture)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(create_game_texture(in_name, in_texture));
	}

	//===================================================
	//                    Flipbook
	//===================================================

	namespace
	{
		Scope<Flipbook> create_flipbook(const std::string_view in_name, AssetID<GameTexture> in_texID, const FlipbookDescription& in_desc)
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
		return GetAssetManager().EmplaceAsset(create_flipbook(in_name, in_texID, in_desc));
	}

	AssetID<Flipbook> Flipbook::ReplaceOrCreate(const std::string_view in_name, AssetID<GameTexture> in_texID, const FlipbookDescription& in_desc)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(create_flipbook(in_name, in_texID, in_desc));
	}

	//===================================================
	//                      Sprite
	//===================================================

	namespace
	{
		Scope<Sprite> create_sprite(const std::string_view in_name, AssetID<GameTexture> in_texID, const SpriteDescription& in_desc)
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
		return GetAssetManager().EmplaceAsset(create_sprite(in_name, in_texID, in_prop));
	}

	AssetID<Sprite> Sprite::ReplaceOrCreate(const std::string_view in_name, AssetID<GameTexture> in_texID, const SpriteDescription& in_prop)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(create_sprite(in_name, in_texID, in_prop));
	}

	//===================================================
	//                      Mesh
	//===================================================

	namespace
	{
		Scope<Mesh> create_mesh(const std::string_view in_name, const Ref<Geometry>& in_geometry)
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
		return GetAssetManager().EmplaceAsset(create_mesh(in_name, in_geometry));
	}

	crab::AssetID<crab::Mesh> Mesh::ReplaceOrCreate(const std::string_view in_name, const Ref<Geometry>& in_geometry)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(create_mesh(in_name, in_geometry));
	}

	//===================================================
	//			         Mono Script
	//===================================================

	namespace
	{
		Scope<MonoScript> create_mono_script(
			const std::string_view in_name, 
			const std::filesystem::path& in_path,
			std::string_view in_namespace, 
			const std::string_view in_className
		)
		{
			Scope<MonoScript> script = MakeScope<MonoScript>();
			script->name = in_name;
			script->scriptPath = in_path;
			script->namespaceName = in_namespace;
			script->className = in_className;
			script->type = eAssetType::MonoScript;

			return script;
		}
	}

	AssetID<MonoScript> MonoScript::Create(
		const std::string_view in_name, 
		const std::filesystem::path& in_path,
		std::string_view in_namespace,
		const std::string_view in_className
	)
	{
		return GetAssetManager().EmplaceAsset(create_mono_script(in_name, in_path, in_namespace, in_className));
	}

	AssetID<MonoScript> MonoScript::ReplaceOrCreate(
		const std::string_view in_name, 
		const std::filesystem::path& in_path,
		std::string_view in_namespace,
		const std::string_view in_className
	)
	{
		return GetAssetManager().ReplaceOrEmplaceAsset(create_mono_script(in_name, in_path, in_namespace, in_className));
	}
}

