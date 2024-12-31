#include "CrabEnginePch.h"
#include "Scene.h"
#include "Components.h"

namespace crab
{
	std::filesystem::path Scene::GetSceneDataPath() const
	{
		return std::filesystem::current_path() / "Scenes" / fmt::format("{}.json", ToString()).c_str();
	}

	Entity Scene::CreateEntity()
	{
		entt::entity e = m_registry.create();
		m_registry.emplace<Transform>(e);
		return Entity{ m_registry, e };
	}

}