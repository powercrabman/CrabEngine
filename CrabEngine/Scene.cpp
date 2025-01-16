#include "CrabEnginePch.h"
#include "Scene.h"
#include "Components.h"
#include "NativeScriptSystem.h"
#include "SceneSerializer.h"

namespace crab
{
	void Scene::OnSetupSceneSuper()
	{
		// scene 파일 로딩
		if (std::filesystem::exists(GetSceneDataPath()))
		{
			SceneSerializer serializer;
			serializer.LoadJsonFromFile(GetSceneDataPath());
			serializer.LoadSceneDataFromJson(this);
		}

		OnSetupScene();
		m_isSetuped = true;
	}

	void Scene::OnEnterSceneSuper()
	{
		if (!m_isSetuped)
		{
			OnSetupSceneSuper();
		}

		MonoScriptEngine::OnEnterScene();
		OnEnterScene();
	}

	void Scene::OnUpdateSuper(TimeStamp& in_ts)
	{
		MonoScriptEngine::OnUpdate(in_ts);
		OnUpdateNativeScriptSystem(this, in_ts);
		OnUpdateFlipbookSystem(this, in_ts);
		OnUpdate(in_ts);
	}

	void Scene::OnRenderSuper(TimeStamp& in_ts)
	{
		OnRenderFlipbookSystem(this);
		OnRenderSpriteSystem(this);
		OnRenderSphereSystem(this);
		OnRenderBoxSystem(this);

		OnRender(in_ts);
	}

	void Scene::OnEventSuper(IEvent& in_event)
	{
		OnEvent(in_event);
	}

	void Scene::OnExitSceneSuper()
	{
		OnExitScene();
	}

	std::filesystem::path Scene::GetSceneDataPath() const
	{
		return std::filesystem::current_path() / "Scenes" / fmt::format("{}.json", GetName()).c_str();
	}

	Entity Scene::CreateEntity()
	{
		entt::entity e = m_registry.create();
		m_registry.emplace<Transform>(e);
		return Entity{ m_registry, e };
	}

	Entity Scene::CreateEntity(uint32 in_id)
	{
		entt::entity e = m_registry.create(entt::entity{in_id});
		m_registry.emplace<Transform>(e);
		return Entity{ m_registry, e };
	}

	void Scene::DeleteEntity(Entity in_entity)
	{
		if (in_entity.IsValid())
		{
			m_registry.destroy(in_entity);
		}
	}

	Entity Scene::DuplicateEntity(Entity in_srcEntity)
	{
		entt::entity newEntity = m_registry.create();
		for (auto [id, set] : m_registry.storage())
		{
			if (set.contains(in_srcEntity))
			{
				set.push(newEntity, set.value(in_srcEntity));
			}
		}
		return Entity{ this, newEntity };
	}

	Entity Scene::TryFindByID(uint32 in_id)
	{
		entt::entity entity = static_cast<entt::entity>(in_id);
		if (m_registry.valid(entity))
		{
			return Entity{ m_registry, entity };
		}
		else
		{
			return Entity::s_null;
		}
	}
}
