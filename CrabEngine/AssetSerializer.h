#pragma once
#include "AssetSingleInclude.h"
#include "JsonSerializer.h"

namespace crab
{
	// GameTexture Serializer Not Implement (not necessary)

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

	class IAssetSerializer
	{
	public:
		virtual void CreateFromJson(const Json& in_json) = 0;
		virtual void ReplaceFromJson(const Json& in_json) = 0;
		virtual void SaveToJson(Json& in_out_json, const AssetBase* in_asset) = 0;
	};

	//===================================================
	//                      Sprite
	//===================================================

	SERIALIZERABLE(SpriteDescription, pivotCol, pivotRow, width, height);
	static_assert(sizeof(SpriteDescription) == 16);

	class SpriteSerializer : public IAssetSerializer
	{
	public:
		void CreateFromJson(const Json& in_json) override
		{
			std::string name;
			AssetID<GameTexture> id;
			SpriteDescription desc;
			load_game_texture(in_json, name, id, desc);

			Sprite::Create(name, id, desc);
		}

		void ReplaceFromJson(const Json& in_json) override
		{
			std::string name;
			AssetID<GameTexture> id;
			SpriteDescription desc;
			load_game_texture(in_json, name, id, desc);

			Sprite::ReplaceOrCreate(name, id, desc);
		}

		void SaveToJson(Json& in_out_json, const AssetBase* in_asset) override
		{
			const Sprite* sprite = static_cast<const Sprite*>(in_asset);
			in_out_json["type"] = eAssetType::Sprite;
			in_out_json["name"] = sprite->name;
			in_out_json["description"] = sprite->desc;

			const GameTexture* tex = TryGetAsset(sprite->textureID);
			if (!tex) assert(false);
			in_out_json["texture name"] = tex->name;
		}

	private:
		void load_game_texture(const Json& in_json, std::string& out_name, AssetID<GameTexture>& out_id, SpriteDescription& out_desc)
		{
			assert(!in_json.is_null());
			out_id = GetAssetManager().TryFindAssetIDByName<GameTexture>(in_json["texture name"]);
			if (!IsValidAsset(out_id)) assert(false);
			out_name = in_json["name"];
			out_desc = in_json["description"];
		}
	};

	//===================================================
	//                     Flipbook
	//===================================================

	SERIALIZERABLE(FlipbookDescription, pivotCol, pivotRow, width, height, totalFrame, duration, loop);
	static_assert(sizeof(FlipbookDescription) == 28);

	class FlipbookSerializer : public IAssetSerializer
	{
	public:
		void CreateFromJson(const Json& in_json) override
		{
			std::string name;
			AssetID<GameTexture> id;
			FlipbookDescription desc;
			load_flipbook(in_json, name, id, desc);

			Flipbook::Create(name, id, desc);
		}

		void ReplaceFromJson(const Json& in_json) override
		{
			std::string name;
			AssetID<GameTexture> id;
			FlipbookDescription desc;
			load_flipbook(in_json, name, id, desc);

			Flipbook::ReplaceOrCreate(name, id, desc);
		}

		void SaveToJson(Json& in_out_json, const AssetBase* in_asset) override
		{
			const Flipbook* flipbook = static_cast<const Flipbook*>(in_asset);
			in_out_json["type"] = eAssetType::Flipbook;
			in_out_json["name"] = flipbook->name;
			in_out_json["description"] = flipbook->desc;

			const GameTexture* tex = TryGetAsset(flipbook->textureID);
			if (!tex) assert(false);
			in_out_json["texture name"] = tex->name;
		}

	private:
		void load_flipbook(const Json& in_json, std::string& out_name, AssetID<GameTexture>& out_id, FlipbookDescription& out_desc)
		{
			assert(!in_json.is_null());
			out_id = GetAssetManager().TryFindAssetIDByName<GameTexture>(in_json["texture name"]);
			if (!IsValidAsset(out_id)) assert(false);
			out_name = in_json["name"];
			out_desc = in_json["description"];
		}
	};

	//===================================================
	//						Mesh
	//===================================================

	//===================================================
	//			        Mono Script
	//===================================================

	class MonoScriptSerializer : public IAssetSerializer
	{
	public:

		void CreateFromJson(const Json& in_json) override
		{
			std::string name, namespaceName, className;
			load_flipbook(in_json, name, namespaceName, className);
			MonoScript::Create(name, namespaceName, className);
		}

		void ReplaceFromJson(const Json& in_json) override
		{
			std::string name, namespaceName, className;
			load_flipbook(in_json, name, namespaceName, className);
			MonoScript::ReplaceOrCreate(name, namespaceName, className);
		}

		void SaveToJson(Json& in_out_json, const AssetBase* in_asset) override
		{
			const MonoScript* mono = static_cast<const MonoScript*>(in_asset);
			in_out_json["type"] = eAssetType::MonoScript;	
			in_out_json["name"] = mono->name;
			in_out_json["namespace"] = mono->namespaceName;
			in_out_json["class name"] = mono->className;
		}

	private:
		void load_flipbook(const Json& in_json, std::string& out_name, std::string& out_namespace, std::string& out_className)
		{
			assert(!in_json.is_null());
			out_name = in_json["name"];
			out_namespace = in_json["namespace"];
			out_className = in_json["class name"];
		}
	};

	//===================================================
	//                Asset Serializer
	//===================================================

	class AssetSerializer : public JsonSerializerBase
	{
		using Super = JsonSerializerBase;
	public:
		bool LoadJsonFromFile(std::filesystem::path in_path) override
		{
			if (Super::LoadJsonFromFile(in_path))
			{
				eAssetType type = m_json["type"];
				create_serializer_instance(type);
				return true;
			}
			else
			{
				return false;
			}
		}

		void CreateFromJson() const
		{
			assert(m_serializer);
			assert(IsJsonLoaded());
			m_serializer->CreateFromJson(m_json);
		}

		void ReplaceFromJson() const
		{
			assert(m_serializer);
			assert(IsJsonLoaded());
			m_serializer->ReplaceFromJson(m_json);
		}

		void SaveToJson(const AssetBase* in_asset)
		{
			create_serializer_instance(in_asset->type);
			m_serializer->SaveToJson(m_json, in_asset);
		}

	private:
		void create_serializer_instance(crab::eAssetType type)
		{
			switch (type)
			{
			case crab::eAssetType::Flipbook:
				m_serializer = MakeScope<FlipbookSerializer>();
				break;

			case crab::eAssetType::Sprite:
				m_serializer = MakeScope<SpriteSerializer>();
				break;

			case crab::eAssetType::MonoScript:
				m_serializer = MakeScope<MonoScriptSerializer>();
				break;

			default:
				assert(false);
				break;
			}
		}


		Scope<IAssetSerializer>	m_serializer = nullptr;
	};
}