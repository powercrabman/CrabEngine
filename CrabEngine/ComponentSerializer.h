#pragma once
#include "AssetManager.h"
#include "AssetSingleInclude.h"
#include "PhysicComponents.h"

namespace crab
{
	static_assert(sizeof(Tag) == 40);
	static_assert(sizeof(Transform) == 36);
	static_assert(sizeof(Camera) == 16);
	static_assert(sizeof(SpriteRender) == 24);
	static_assert(sizeof(FlipbookRender) == 40);
	static_assert(sizeof(SphereRender) == 20);
	static_assert(sizeof(BoxRender) == 24);
	static_assert(sizeof(RigidBody2D) == 16);
	static_assert(sizeof(Collider2DData) == 12);
	static_assert(sizeof(BoxCollider2D) == 20);
	static_assert(sizeof(SphereCollider2D) == 24);
	static_assert(sizeof(CapsuleCollider2D) == 32);
	static_assert(sizeof(BoundingBox) == 24);
	static_assert(sizeof(OrientBoundingBox) == 40);
	static_assert(sizeof(MonoScriptRunner) == 8);

	ENUM_SERIALIZERABLE(eBodyType, {
	{eBodyType::Static, "Static"},
	{eBodyType::Kinematic, "Kinematic"},
	{eBodyType::Dynamic, "Dynamic"}
	});

	SERIALIZERABLE(Tag, name);
	SERIALIZERABLE(Transform, position, rotation, scaling);
	SERIALIZERABLE(Collider2DData, friction, density, restitution);
	SERIALIZERABLE(RigidBody2D, type, fixRotation);
	SERIALIZERABLE(Camera, mode, nearPlane, farPlane, fov);
	SERIALIZERABLE(SphereCollider2D, colliderData, center, radius);
	SERIALIZERABLE(CapsuleCollider2D, colliderData, center1, center2, radius);
	SERIALIZERABLE(BoxCollider2D, colliderData, size);
	SERIALIZERABLE(BoundingBox, box);
	SERIALIZERABLE(OrientBoundingBox, box);

	//===================================================
	//                  Sprite Render
	//===================================================

	inline void to_json(Json& in_out_json, const SpriteRender& in_data)
	{
		if (TryGetAsset(in_data.spriteID))
		{
			in_out_json["asset name"] = TryGetAsset(in_data.spriteID)->name;
		}
		else
		{
			in_out_json["asset name"] = "null";
		}
		in_out_json["color"] = in_data.color;
	}

	inline void from_json(const Json& in_json, SpriteRender& in_out_data)
	{
		AssetID<Sprite> id = GetAssetManager().TryFindAssetIDByName<Sprite>(in_json["asset name"]);
		const Sprite* sp = TryGetAsset(id);
		if (sp)
		{
			in_out_data.spriteID = id;
		}

		in_out_data.color = in_json["color"];
	}

	//===================================================
	//                 Flipbook Render
	//===================================================

	inline void to_json(Json& in_out_json, const FlipbookRender& in_data)
	{
		if (TryGetAsset(in_data.assetID))
		{
			in_out_json["asset name"] = TryGetAsset(in_data.assetID)->name;
		}
		else
		{
			in_out_json["asset name"] = "null";
		}
		in_out_json["playbackSpeed"] = in_data.playbackSpeed;
		in_out_json["color"] = in_data.color;
	}

	inline void from_json(const Json& in_json, FlipbookRender& in_out_data)
	{
		AssetID<Flipbook> id = GetAssetManager().TryFindAssetIDByName<Flipbook>(in_json["asset name"]);
		const Flipbook* fb = TryGetAsset(id);
		if (fb)
		{
			in_out_data.assetID = id;
		}
		in_out_data.playbackSpeed = in_json["playbackSpeed"];
		in_out_data.color = in_json["color"];
	}

	//===================================================
	//			         Mono Script
	//===================================================

	inline void to_json(Json& in_out_json, const MonoScriptRunner& in_data)
	{
		if (TryGetAsset(in_data.assetID))
		{
			in_out_json["asset name"] = TryGetAsset(in_data.assetID)->name;
		}
		else
		{
			in_out_json["asset name"] = "null";
		}
	}

	inline void from_json(const Json& in_json, MonoScriptRunner& in_out_data)
	{
		AssetID<MonoScript> id = GetAssetManager().TryFindAssetIDByName<MonoScript>(in_json["asset name"]);
		const MonoScript* script = TryGetAsset(id);
		if (script)
		{
			in_out_data.assetID = id;
		}
	}
}
