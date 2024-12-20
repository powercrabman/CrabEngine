#include "CrabEnginePch.h"
#include "Scene.h"

namespace crab
{

	crab::Entity Scene::CreateEntity()
	{
		entt::entity e = m_registry.create();
		return Entity{ m_registry, e };
	}

}