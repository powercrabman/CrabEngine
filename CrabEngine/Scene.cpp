#include "CrabEnginePch.h"
#include "Scene.h"
#include "Components.h"

namespace crab
{
	Entity Scene::CreateEntity()
	{
		entt::entity e = m_registry.create();
		m_registry.emplace<Transform>(e);
		return Entity{ m_registry, e };
	}

}