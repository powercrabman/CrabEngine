#pragma once
#include "Entity.h"

namespace crab
{

#define REGIST_SCENE(scene)\
	inline static bool _regist_scene_ = []() { crab::SceneManager::Get().CreateScene<scene>(); return true; }();\
	inline static const char* s_sceneName = #scene


	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;

		// Core
		virtual void OnEnterScene() = 0;
		virtual void OnExitScene() = 0;
		virtual void OnUpdate(float in_deltaTime) = 0;
		virtual void OnRender(float in_deltaTime) = 0;
		virtual void OnImGuiRender(float in_deltaTime) = 0;

		// About ECS
		Entity			CreateEntity();
		bool			DeleteEntity(const Entity& in_entity) { m_registry.destroy(in_entity); }

		entt::registry& GetRegistry() { return m_registry; }

		template<typename ...Types>
		auto GetView() { return m_registry.view<Types...>(); }

	private:
		entt::registry m_registry;
	};
}
