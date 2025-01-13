#pragma once
#include "ComponentSerializerManager.h"
#include "NativeScript.h"
#include "Types.h"

namespace crab
{
	//===================================================
	//                     Base
	//===================================================

#define COMPONENT_BODY(CompName)\
		inline static const char* s_staticName = #CompName;\
		inline static bool _regist_comp_serializer_ = []() {crab::ComponentSerializerManager::Get().RegisterSerializerCallback<CompName>(); return true; }()

	//===================================================
	//						Tag
	//===================================================

	struct Tag
	{
		std::string name;

		COMPONENT_BODY(Tag);
	};

	//===================================================
	//                   Transform
	//===================================================

	// LH Coordinate
	struct Transform
	{
		__forceinline Vec3 ForwardVector() const { return Mat::CreateFromYawPitchRoll(rotation).Backward(); }
		__forceinline Vec3 BackwardVector() const { return Mat::CreateFromYawPitchRoll(rotation).Forward(); }
		__forceinline Vec3 LeftVector() const { return Mat::CreateFromYawPitchRoll(rotation).Left(); }
		__forceinline Vec3 RightVector() const { return Mat::CreateFromYawPitchRoll(rotation).Right(); }
		__forceinline Vec3 UpVector() const { return Mat::CreateFromYawPitchRoll(rotation).Up(); }
		__forceinline Vec3 DownVector() const { return Mat::CreateFromYawPitchRoll(rotation).Down(); }

		__forceinline Mat GetWorld() const
		{
			return DirectX::XMMatrixAffineTransformation(
				scaling,
				Vec3::Zero,
				DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(rotation),
				position
			);
		}

		Vec3 position = Vec3::Zero;
		Vec3 rotation = Vec3::Zero; // Yaw, Roll, Pitch (Radian)
		Vec3 scaling = Vec3::One;

		static Transform s_identity;

		COMPONENT_BODY(Transform);
	};

	//===================================================
	//                    Camera
	//===================================================

	struct Camera
	{
		__forceinline Mat GetView(const Transform& in_transform) const
		{
			Mat rotMat = DirectX::XMMatrixRotationRollPitchYaw(in_transform.rotation.x, in_transform.rotation.y, in_transform.rotation.z);
			return DirectX::XMMatrixLookToLH(
				in_transform.position,
				rotMat.Backward(),
				rotMat.Up()
			);
		}

		__forceinline Mat GetProj(const Transform& in_transform, float in_aspect) const
		{
			if (mode == eProjection::Orthographic)
			{
				return DirectX::XMMatrixOrthographicLH(-in_transform.position.z * in_aspect, -in_transform.position.z, nearPlane, farPlane);
			}
			else
			{
				return DirectX::XMMatrixPerspectiveFovLH(fov, in_aspect, nearPlane, farPlane);
			}
		}

		__forceinline Mat GetViewProj(const Transform& in_transform, float in_aspect) const
		{
			return GetView(in_transform) * GetProj(in_transform, in_aspect);
		}

		eProjection mode = eProjection::Orthographic;
		float		nearPlane = 0.01f;
		float		farPlane = 100.f;
		float		fov = math::DegToRad(45.f);

		COMPONENT_BODY(Camera);
	};

	//===================================================
	//                    Render
	//===================================================

	//struct MeshRender
	//{
	//	AssetID<Mesh>	spriteID = NullAsset<Mesh>;
	//	Vec4			color = Vec4::Zero;
	//
	//	COMPONENT_BODY(MeshRender);
	//};

	struct SphereRender
	{
		float	radius = 0.5f;
		Vec4	color = Color::WHITE;

		COMPONENT_BODY(SphereRender);
		SERIALIZERALBE_INTRUSIVE(SphereRender, radius, color);
	};

	struct BoxRender
	{
		Vec2	size = Vec2::One;
		Vec4	color = Color::WHITE;

		COMPONENT_BODY(BoxRender);
		SERIALIZERALBE_INTRUSIVE(BoxRender, size, color);
	};

	struct SpriteRender
	{
		AssetID<Sprite> spriteID = NullAsset<Sprite>;
		Vec4			color = Color::WHITE;

		COMPONENT_BODY(SpriteRender);
	};

	struct FlipbookRender
	{
		AssetID<Flipbook>	assetID = NullAsset<Flipbook>;
		Vec4				color = Color::WHITE;
		uint32				currentFrame = 0;
		float				timeAcc = 0.f;

		float				playbackSpeed = 1.f;
		bool				isFinish = false;
		bool				isPlay = true;

		COMPONENT_BODY(FlipbookRender);
	};

	//===================================================
	//			       Bounding Box
	//===================================================

	struct BoundingBox
	{
		AABB box;

		COMPONENT_BODY(BoundingBox);
	};

	struct OrientBoundingBox
	{
		OBB box;

		COMPONENT_BODY(OrientBoundingBox);
	};

	//===================================================
	//	Native Script (C++ scripting not serializable)
	//===================================================

	struct NativeScriptRunner
	{
		Scope<NativeScript> script;
		bool				isInited = false;
	};

	//===================================================
	//			  Mono Script (C# scripting)
	//===================================================

	struct MonoScriptRunner
	{
		AssetID<MonoScript> assetID;

		COMPONENT_BODY(MonoScriptRunner);
	};

	//===================================================
	//			     Meta Programming
	//===================================================

	template<typename... Components>
	struct ComponentGroup
	{
	};

	using AllComponents =
		ComponentGroup<
		Tag, Transform, Camera,
		SphereRender, BoxRender, SpriteRender,
		FlipbookRender, BoundingBox,
		OrientBoundingBox, MonoScriptRunner
		>;
}
