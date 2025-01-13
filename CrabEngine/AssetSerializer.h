#pragma once
#include "AssetSingleInclude.h"
#include "JsonSerializer.h"

namespace crab
{
	//===================================================
	//                      Base
	//===================================================

	ENUM_SERIALIZERABLE(eAssetType, {
		{ eAssetType::GameTexture, "GameTexture" },
		{ eAssetType::Flipbook, "Flipbook" },
		{ eAssetType::Sprite, "Sprite" },
		{ eAssetType::Mesh, "Mesh" },
		{ eAssetType::MonoScript, "MonoScript" }
		});

	//===================================================
	//			         GameTexture
	//===================================================

	// Not Implement (not necessary)

	//===================================================
	//                      Sprite
	//===================================================

	SERIALIZERABLE(SpriteDescription, pivotCol, pivotRow, width, height);
	static_assert(sizeof(SpriteDescription) == 16);

	class SpriteSerializer
	{
	public:
		void CreateFromJson(const Json& in_json)
		{
			AssetID<GameTexture> id = GetAssetManager().TryFindAssetIDByName<GameTexture>(in_json["texture name"]);
			if (!IsValidAsset(id)) assert(false);
			SpriteDescription desc = in_json["description"];
			Sprite::ReplaceOrCreate(in_json["name"], id, desc);
		}

		void ToJson(Json& in_out_json, const Sprite* in_sprite)
		{
			in_out_json["type"] = eAssetType::Sprite;
			in_out_json["name"] = in_sprite->name;
			in_out_json["description"] = in_sprite->desc;

			const GameTexture* tex = TryGetAsset(in_sprite->textureID);
			if (!tex) assert(false);
			in_out_json["texture name"] = tex->name;
		}
	};

	//===================================================
	//                     Flipbook
	//===================================================

	SERIALIZERABLE(FlipbookDescription, pivotCol, pivotRow, width, height, totalFrame, duration, loop);
	static_assert(sizeof(FlipbookDescription) == 28);

	class FlipbookSerializer
	{
	public:
		void CreateFromJson(const Json& in_json)
		{
			AssetID<GameTexture> id = GetAssetManager().TryFindAssetIDByName<GameTexture>(in_json["texture name"]);
			if (!IsValidAsset(id)) assert(false);
			FlipbookDescription desc = in_json["description"];
			Flipbook::ReplaceOrCreate(in_json["name"], id, desc);
		}

		void ToJson(Json& in_out_json, const Flipbook* in_flipbook)
		{
			in_out_json["type"] = eAssetType::Flipbook;
			in_out_json["name"] = in_flipbook->name;
			in_out_json["description"] = in_flipbook->desc;

			const GameTexture* tex = TryGetAsset(in_flipbook->textureID);
			if (!tex) assert(false);
			in_out_json["texture name"] = tex->name;
		}
	};

	//===================================================
	//						Mesh
	//===================================================

	//===================================================
	//			        Mono Script
	//===================================================

	class MonoScriptSerializer
	{
	public:
		void CreateFromJson(const Json& in_json)
		{
			MonoScript::ReplaceOrCreate(in_json["name"], in_json["namespace"], in_json["class name"]);
		}

		void ToJson(Json& in_out_json, const MonoScript* in_mono)
		{
			in_out_json["name"] = in_mono->name;
			in_out_json["namespace"] = in_mono->namespaceName;
			in_out_json["class name"] = in_mono->className;
		}
	};

	//===================================================
	//                 Asset Serializer
	//===================================================

	class AssetSerializer : public JsonSerializerBase
	{
	public:
		void CreateFromJson()
		{
			eAssetType type = m_json["type"];
			switch (type)
			{

			case eAssetType::Flipbook:
			{
				FlipbookSerializer serializer;
				serializer.CreateFromJson(m_json);
			}
			break;

			case eAssetType::Sprite:
			{
				SpriteSerializer serializer;
				serializer.CreateFromJson(m_json);
			}
			break;

			case eAssetType::MonoScript:
			{
				MonoScriptSerializer serializer;
				serializer.CreateFromJson(m_json);
			}
			break;

			default:
				assert(false);
				break;
			}
		}

		template<typename AssetType>
		void ToJson(AssetType* in_assetData)
		{
			if constexpr (IS_SAME_TYPE(AssetType, Sprite))
			{
				SpriteSerializer serializer;
				serializer.ToJson(m_json, in_assetData);
			}
			else if constexpr (IS_SAME_TYPE(AssetType, Flipbook))
			{
				FlipbookSerializer serializer;
				serializer.ToJson(m_json, in_assetData);
			}
			else if constexpr (IS_SAME_TYPE(AssetType, MonoScript))
			{
				MonoScriptSerializer serializer;
				serializer.ToJson(m_json, in_assetData);
			}
		}
	};
}