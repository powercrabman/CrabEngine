#pragma once
#include <box2d/id.h>
#include <box2d/types.h>

namespace crab
{
	constexpr uint32 PHYSIC_COLOR = 0xff4b0a; // 빨간색

	//===================================================
	//                      Physic
	//===================================================

	enum class eBodyType 
	{
		Static = 0, 	// 정적 강체, 물리적 영향을 받지 않음. 땅과 벽에 사용. 계산량이 적음
		Kinematic, 		// 운동학 강체, 물리적 영향을 받지 않음. 그러나 사용자가 지정한 대로 임의의 운동이 가능함
		Dynamic			// 동적 강체, 물리적 영향을 받음. 캐릭터, 적, 총알 등 물리적 영향을 받는 엔터티에 사용
	}; // '물리적 영향' 이란 중력, 마찰력, 충돌, 반발력 등을 의미

	static_assert(static_cast<uint32>(eBodyType::Static)    == static_cast<uint32>(b2BodyType::b2_staticBody));
	static_assert(static_cast<uint32>(eBodyType::Kinematic) == static_cast<uint32>(b2BodyType::b2_kinematicBody));
	static_assert(static_cast<uint32>(eBodyType::Dynamic)   == static_cast<uint32>(b2BodyType::b2_dynamicBody));

	//===================================================
	//			            Rigid
	//===================================================

	struct RigidBody2D
	{
		eBodyType			type         = eBodyType::Static;	// 타입 (자세한 것은 eBodyType 참조)
		bool				fixRotation  = true;				// 회전 고정
		b2BodyId			internalData;						// Don't touch (no serialize)

		COMPONENT_BODY(RigidBody2D);
	};

	//===================================================
	//                     Collider
	//===================================================

	struct Collider2DData
	{
		float				friction     = 0.6f;			// 마찰계수
		float				density      = 1.f;				// 밀도 (단위 면적 당 질량)
		float				restitution  = 0.f;				// 반발력 (탄성계수)
	};

	struct BoxCollider2D
	{
		Collider2DData		colliderData;			// 충돌 데이터
		Vec2				size = { 0.5f, 0.5f };	// 중심

		COMPONENT_BODY(BoxCollider2D);
	};

	struct SphereCollider2D
	{
		Collider2DData		colliderData;				// 충돌 데이터
		Vec2				center = { 0.f, 0.f };		// 중심
		float				radius = 0.5f;				// 반지름

		COMPONENT_BODY(SphereCollider2D);
	};

	struct CapsuleCollider2D
	{
		Collider2DData		colliderData;				// 충돌 데이터
		Vec2				center1 = { -0.5f, 0.f };	// 중심1
		Vec2				center2 = { 0.5f, 0.f };	// 중심2
		float				radius = 1.f;				// 반지름

		COMPONENT_BODY(CapsuleCollider2D);
	};
}
