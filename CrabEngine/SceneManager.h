#pragma once
#include "Scene.h"

namespace crab
{
	class SceneManager : public Singleton<SceneManager>
	{
		friend class Singleton<SceneManager>;
	public:
		template <typename Ty>
		Scene*  CreateScene();

		template <typename Ty>
		Scene*	TryFindScene();
		Scene*	TryFindSceneByName(const std::string_view in_name);
		Scene*	TryGetCurrentScene() const;

		template <typename Ty>
		void	DeleteScene();

		template <typename Ty>
		void	ChangeScene();
		void	ChangeSceneByName(const std::string_view in_name) { _change_scene_(TryFindSceneByName(in_name)); }

		void	OnUpdate(float in_deltaTime);
		void	OnRender(float in_deltaTime);
		void	OnImGuiRender(float in_deltaTime);

		void	OnEvent(IEvent& in_event);

		std::unordered_map<TypeID, Scope<Scene>>::const_iterator cbegin() const { return m_sceneRepo.cbegin(); }
		std::unordered_map<TypeID, Scope<Scene>>::const_iterator cend() const { return m_sceneRepo.cend(); }

		std::unordered_map<TypeID, Scope<Scene>>::iterator begin() { return m_sceneRepo.begin(); }
		std::unordered_map<TypeID, Scope<Scene>>::iterator end() { return m_sceneRepo.end(); }

	private:
		void _change_scene_(Scene* in_scene);
		void _setup_scene_(Scene* in_scene);

		std::unordered_map<TypeID, Scope<Scene>>	m_sceneRepo;
		Scene*										m_currentScene = nullptr;
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
		_change_scene_(scene);
	}

}