#pragma once
#include "Entity.h"

namespace crab
{

#define REGIST_SCENE(scene)\
	const char* ToString() const override {return #scene; };\
	inline static bool _regist_scene_ = []() { crab::SceneManager::Get().CreateScene<scene>(); return true; }();\
	inline static const char* s_sceneName = #scene


	class Scene
	{
		friend class SceneManager;
	public:
		Scene() = default;
		virtual ~Scene() = default;

		// Core
		virtual void SetupScene() = 0;
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

		virtual const char* ToString() const = 0;

	private:

		entt::registry	m_registry;
		bool			m_isSetuped = false;
	};
}
