#pragma once
#include "Scene.h"

namespace crab
{
	class SceneManager : public Singleton<SceneManager>
	{
		friend class Singleton<SceneManager>;
	public:
		template <typename Ty>
		Scene*	CreateScene();

		template <typename Ty>
		Scene*  TryFindScene();

		template <typename Ty>
		void	DeleteScene();

		template <typename Ty>
		void	ChangeScene();

		void	OnUpdate(float in_deltaTime);
		void	OnRender(float in_deltaTime);
		void	OnImGuiRender(float in_deltaTime);

		void	OnEvent(IEvent& in_event);

	private:
		std::unordered_map<TypeID, Scope<Scene>> m_sceneRepo;
		Scene* m_currentScene = nullptr;
	};

	//===================================================
	//                      Inline
	//===================================================

	template <typename Ty>
	inline Scene* crab::SceneManager::TryFindScene()
	{
		static_assert(std::is_base_of_v<Scene, Ty>);
		auto iter = m_sceneRepo.find(GenerateTypeID<Ty>());
		assert(iter != m_sceneRepo.end());
		return iter->second.get();
	}

	template <typename Ty>
	inline Scene* crab::SceneManager::CreateScene()
	{
		static_assert(std::is_base_of_v<Scene, Ty>);
		auto [iter, retVal] = m_sceneRepo.emplace(GenerateTypeID<Ty>(), MakeScope<Ty>());
		assert(retVal);
		return iter->second.get();
	}

	template <typename Ty>
	inline void crab::SceneManager::DeleteScene()
	{
		static_assert(std::is_base_of_v<Scene, Ty>);
		auto iter = m_sceneRepo.find(GenerateTypeID<Ty>());
		assert(iter != m_sceneRepo.end());
		m_sceneRepo.erase(iter);
	}

	template<typename Ty>
	inline void SceneManager::ChangeScene()
	{
		static_assert(std::is_base_of_v<Scene, Ty>);
		Scene* scene = TryFindScene<Ty>();

		if (scene)
		{
			if (m_currentScene) { m_currentScene->OnExitScene(); }

			m_currentScene = scene;
			m_currentScene->OnEnterScene();
		}
	}

}