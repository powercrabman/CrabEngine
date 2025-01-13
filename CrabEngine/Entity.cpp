#include "CrabEnginePch.h"
#include "Entity.h"
#include "Scene.h"

namespace crab
{
	Entity Entity::s_null = Entity{ nullptr, entt::null };

	Entity::Entity(Scene* in_scene, entt::entity in_entity)
		: m_registry(in_scene ? &in_scene->GetRegistry() : nullptr)
		, m_entity(in_entity)
	{
	}

	Entity::Entity(entt::registry& in_registry, entt::entity in_entity)
		: m_registry(&in_registry)
		, m_entity(in_entity)
	{
	}

}