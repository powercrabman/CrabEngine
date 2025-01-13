#include "CrabEnginePch.h"
#include "Physic2D.h"

#include <Windows.Devices.Bluetooth.GenericAttributeProfile.h>

#include "PhysicComponents.h"

namespace crab
{
	Physic2DProp Physic2DProp::s_defaultProp = {};

	Physic2D::Physic2D(const Physic2DProp& in_prop)
		: m_prop(in_prop)
	{
		
	}

	Physic2D::~Physic2D()
	{
		DestroyWorld();
	}

	void Physic2D::Shutdown()
	{
		DestroyWorld();
	}

	void Physic2D::OnEnterScene()
	{
		// Create World
		b2WorldDef worldDef            = b2DefaultWorldDef();
		worldDef.gravity               = b2Vec2{ m_prop.gravity.x, m_prop.gravity.y };
		worldDef.contactPushSpeed      = m_prop.contactPushSpeed;
		worldDef.restitutionThreshold  = m_prop.restitutionThreshold;
		worldDef.maximumLinearSpeed    = m_prop.maximumLinearSpeed;
		worldDef.enableSleep           = m_prop.enableSleep;
		worldDef.enableContinuous      = m_prop.enableContinuous;
		worldDef.jointHertz            = m_prop.jointHertz;
		worldDef.contactDampingRatio   = m_prop.contactDampingRatio;
		worldDef.jointDampingRatio     = m_prop.jointDampingRatio;
		worldDef.workerCount           = m_prop.workerCount;
		worldDef.frictionMixingRule    = static_cast<b2MixingRule>(m_prop.frictionMixingRule);
		worldDef.restitutionMixingRule = static_cast<b2MixingRule>(m_prop.restitutionMixingRule);
		worldDef.contactHertz          = m_prop.contactHertz;

		m_worldID = b2CreateWorld(&worldDef);
		Scene* scene = GetSceneManager().TryGetCurrentScene();

		// Create Body
		scene->GetView<RigidBody2D, Transform>().each([&](RigidBody2D& in_rb, const Transform& in_trans)
			{
				b2BodyDef def = b2DefaultBodyDef();
				def.type = static_cast<b2BodyType>(in_rb.type);
				def.position = { in_trans.position.x, in_trans.position.y };

				auto [sin, cos] = math::CalcSinCos(in_trans.rotation.z);
				def.rotation = { cos, sin };
				def.fixedRotation = in_rb.fixRotation;

				in_rb.internalData = b2CreateBody(m_worldID, &def);
			}
		);

		scene->GetView<BoxCollider2D, RigidBody2D, Transform>().each([&](const BoxCollider2D& in_box, const RigidBody2D& in_rb, const Transform& in_trans)
			{
				b2ShapeDef def = b2DefaultShapeDef();
				def.density = in_box.colliderData.density;
				def.friction = in_box.colliderData.friction;
				def.restitution = in_box.colliderData.restitution;
				def.customColor = PHYSIC_COLOR;

				Vec2 finalSize = Vec2{ in_box.size.x * in_trans.scaling.x, in_box.size.y * in_trans.scaling.y } * 0.5f;
				b2Polygon box = b2MakeBox(finalSize.x, finalSize.y);
				b2CreatePolygonShape(in_rb.internalData, &def, &box);
			}
		);

		scene->GetView<SphereCollider2D, RigidBody2D, Transform>().each([&](const SphereCollider2D& in_sph, const RigidBody2D& in_rb, const Transform& in_trans)
			{
				b2ShapeDef def = b2DefaultShapeDef();
				def.density = in_sph.colliderData.density;
				def.friction = in_sph.colliderData.friction;
				def.restitution = in_sph.colliderData.restitution;
				def.customColor = PHYSIC_COLOR;
				
				b2Circle sphere = { {in_sph.center.x, in_sph.center.y}, in_sph.radius * min(in_trans.scaling.x, in_trans.scaling.y) };
				b2CreateCircleShape(in_rb.internalData, &def, &sphere);
			}
		);

		scene->GetView<CapsuleCollider2D, RigidBody2D, Transform>().each([&](const CapsuleCollider2D& in_cap, const RigidBody2D& in_rb, const Transform& in_trans)
			{
				b2ShapeDef def = b2DefaultShapeDef();
				def.density = in_cap.colliderData.density;
				def.friction = in_cap.colliderData.friction;
				def.restitution = in_cap.colliderData.restitution;
				def.customColor = PHYSIC_COLOR;

				b2Capsule capsule = {
					.center1 = {in_cap.center1.x * in_trans.scaling.x, in_cap.center1.y * in_trans.scaling.y},
					.center2 = {in_cap.center2.x * in_trans.scaling.x, in_cap.center2.y * in_trans.scaling.y},
					.radius = in_cap.radius * min(in_trans.scaling.x, in_trans.scaling.y)
				};

				b2CreateCapsuleShape(in_rb.internalData, &def, &capsule);
			}
		);
	}

	void Physic2D::OnUpdateScene(TimeStamp& in_ts)
	{
		b2World_Step(m_worldID, in_ts.deltaTime, m_prop.subStep);

		GetSceneManager().TryGetCurrentScene()->GetView<RigidBody2D, Transform>().each([](const RigidBody2D& in_rb, Transform& in_trans)
			{
				auto [x, y] = b2Body_GetPosition(in_rb.internalData);
				in_trans.position.x = x;
				in_trans.position.y = y;

				if (!in_rb.fixRotation)
				{
					auto [cos, sin] = b2Body_GetRotation(in_rb.internalData);
					in_trans.rotation.z = math::SinCosToRad(sin, cos);
				}
			}
		);

	}

	void Physic2D::OnRenderScene(TimeStamp& in_ts)
	{
#if defined(_DEBUG) || defined(DEBUG)
		
		if (m_prop.enableDebugDraw)
		{
			Scene* scene = GetSceneManager().TryGetCurrentScene();
			scene->GetView<BoxCollider2D, Transform>().each([&](const BoxCollider2D& in_box, const Transform& in_trans)
				{
					auto [s, c] = math::CalcSinCos(in_trans.rotation.z);
					Draw2D::AddRect(
						in_trans.GetWorld(),
						{ in_box.size.x, in_box.size.y },
						m_prop.debugColor, 
						1.f
					);
				});

			scene->GetView<SphereCollider2D, Transform>().each([&](const SphereCollider2D& in_sphere, const Transform& in_trans)
				{
					auto [s, c] = math::CalcSinCos(in_trans.rotation.z);
					Draw2D::AddCircle(
						in_trans.GetWorld(),
						in_sphere.radius,
						m_prop.debugColor, 
						1.f
					);
				});

#endif
		}
	}

	void Physic2D::DestroyWorld() const
	{
		if (b2World_IsValid(m_worldID))
		{
			b2DestroyWorld(m_worldID);
		}
	}

	void Physic2D::OnExitScene() 
	{
		DestroyWorld();
	}
}
