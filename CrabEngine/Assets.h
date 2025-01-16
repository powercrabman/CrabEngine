#pragma once
#include "MonoScriptEngine.h"

namespace crab
{
	//===================================================
	//                     Base
	//===================================================

	struct AssetBase
	{
		std::string name;
		eAssetType	type;
	};

	//===================================================
	//			       Texture Asset
	//===================================================

	struct GameTexture : public AssetBase
	{
		static AssetID<GameTexture> Create(const std::string_view in_name, const std::filesystem::path& in_path);
		static AssetID<GameTexture> Create(const std::string_view in_name, const Ref<ITexture>& in_texture);
		static AssetID<GameTexture> ReplaceOrCreate(const std::string_view in_name, const std::filesystem::path& in_path);
		static AssetID<GameTexture> ReplaceOrCreate(const std::string_view in_name, const Ref<ITexture>& in_texture);
		inline static eAssetType s_s_staticType = eAssetType::GameTexture;

		Ref<ITexture> texture;
	};

	//===================================================
	//                      Mesh
	//===================================================

	struct Mesh : public AssetBase
	{
		static AssetID<Mesh> Create(const std::string_view in_name, const Ref<Geometry>& in_geometry);
		static AssetID<Mesh> ReplaceOrCreate(const std::string_view in_name, const Ref<Geometry>& in_geometry);
		inline static eAssetType s_s_staticType = eAssetType::Mesh;
	
		Ref<Geometry> geometry;
	};

	//===================================================
	//                     Flipbook
	//===================================================

	struct FlipbookDescription
	{
		uint32	pivotCol   = 0;
		uint32	pivotRow   = 0;
		uint32	width      = 0;
		uint32	height     = 0;
		uint32	totalFrame = 1;
		float	duration   = 1.f;
		bool	loop       = false;
	};

	struct Flipbook : public AssetBase
	{
		static AssetID<Flipbook> Create(const std::string_view in_name, AssetID<GameTexture> in_id, const FlipbookDescription& in_desc);
		static AssetID<Flipbook> ReplaceOrCreate(const std::string_view in_name, AssetID<GameTexture> in_id, const FlipbookDescription& in_desc);
		inline static eAssetType s_s_staticType = eAssetType::Flipbook;

		AssetID<GameTexture> textureID = NullAsset<GameTexture>;
		FlipbookDescription desc;
	};

	//===================================================
	//                     Sprite
	//===================================================

	struct SpriteDescription
	{
		uint32	pivotCol = 0;
		uint32	pivotRow = 0;
		uint32	width    = 0;			// 0이면 텍스처의 전체 width 사용
		uint32	height   = 0;			// 0이면 텍스처의 전체 height 사용
	};

	struct Sprite : public AssetBase
	{
		static AssetID<Sprite> Create(const std::string_view in_name, AssetID<GameTexture> in_id, const SpriteDescription& in_desc);
		static AssetID<Sprite> ReplaceOrCreate(const std::string_view in_name, AssetID<GameTexture> in_id, const SpriteDescription& in_desc);
		inline static eAssetType s_s_staticType = eAssetType::Sprite;

		AssetID<GameTexture> textureID = NullAsset<GameTexture>;
		SpriteDescription desc;
	};

	//===================================================
	//                   Mono Script
	//===================================================

	struct MonoScript : public AssetBase
	{
		static AssetID<MonoScript> Create(const std::string_view in_name, std::string_view in_namespace, const std::string_view in_className);
		static AssetID<MonoScript> ReplaceOrCreate(const std::string_view in_name, std::string_view in_namespace, const std::string_view in_className);
		inline static eAssetType s_s_staticType = eAssetType::MonoScript;

		std::string		namespaceName;
		std::string		className;
		MonoEntity		internalData; // Don't touch
	};


}


