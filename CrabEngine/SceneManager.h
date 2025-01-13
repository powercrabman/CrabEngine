#pragma once
#include "IPlugin.h"
#include "Scene.h"

namespace crab
{
	class IPlugin;

	class SceneManager : public Singleton<SceneManager>
	{
		friend class Singleton<SceneManager>;
	public:
		void	Shutdown() const;

		template <typename Ty>
		Scene* EmplaceScene();

		template <typename Ty>
		Scene* TryFindScene();
		Scene* TryFindSceneByName(const std::string_view in_name);
		Scene* TryGetCurrentScene() const;

		template <typename Ty>
		void	DeleteScene();

		template <typename Ty>
		void	ChangeScene();
		void	ChangeScene(Scene* in_scene) { if (in_scene) { _change_scene_(in_scene); } }
		void	ChangeSceneByName(const std::string_view in_name) { _change_scene_(TryFindSceneByName(in_name)); }

		void	RestartScene() { if (m_currentScene) { _change_scene_(m_currentScene); } }

		// Scene Loop
		void	OnUpdate(TimeStamp& in_ts) const;
		void	OnRender(TimeStamp& in_ts) const;
		void	OnImGuiRender(TimeStamp& in_ts) const;

		void	OnEvent(IEvent& in_event) const;

		// IPlugin
		template <class Ty, class ... Args>
		IPlugin* EmplacePlugin(Args... args);

		template <typename Ty>
		IPlugin* TryFindPlugin();

		std::unordered_map<TypeID, Scope<Scene>>::const_iterator cbegin() const { return m_sceneRepo.cbegin(); }
		std::unordered_map<TypeID, Scope<Scene>>::const_iterator cend() const { return m_sceneRepo.cend(); }

		std::unordered_map<TypeID, Scope<Scene>>::iterator begin() { return m_sceneRepo.begin(); }
		std::unordered_map<TypeID, Scope<Scene>>::iterator end() { return m_sceneRepo.end(); }

	private:
		void _change_scene_(Scene* in_scene);

		std::unordered_map<TypeID, Scope<Scene>>	m_sceneRepo;
		Scene* m_currentScene = nullptr;

		std::vector<Scope<IPlugin>>					m_plugins;
	};

	//===================================================
	//                      Inline
	//===================================================

	template <typename Ty>
	inline Scene* crab::SceneManager::TryFindScene()
	{
		static_assert(std::is_base_of_v<Scene, Ty>);
		auto iter = m_sceneRepo.find(TypeID::Generate<Ty>());
		assert(iter != m_sceneRepo.end());
		return iter->second.get();
	}

	template <typename Ty>
	inline Scene* crab::SceneManager::EmplaceScene()
	{
		static_assert(std::is_base_of_v<Scene, Ty>);
		auto [iter, retVal] = m_sceneRepo.emplace(TypeID::Generate<Ty>(), MakeScope<Ty>());
		assert(retVal);
		return iter->second.get();
	}

	template <typename Ty>
	inline void crab::SceneManager::DeleteScene()
	{
		static_assert(std::is_base_of_v<Scene, Ty>);
		auto iter = m_sceneRepo.find(TypeID::Generate<Ty>());
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

	template <typename Ty, typename...Args>
	IPlugin* SceneManager::EmplacePlugin(Args... args)
	{
		IPlugin* retVal = TryFindPlugin<Ty>();
		if (!retVal)
		{
			m_plugins.emplace_back(MakeScope<Ty>(std::forward<Args>(args)...));
			retVal = m_plugins.back().get();
			retVal->Init();
		}
		return retVal;
	}

	template <typename Ty>
	IPlugin* SceneManager::TryFindPlugin()
	{
		auto iter = std::find_if(m_plugins.begin(), m_plugins.end(), [](const Scope<IPlugin>& in_plugin)
			{
				return in_plugin->GetTypeID() == TypeID::Generate<Ty>();
			}
		);

		if (iter == m_plugins.end())
		{
			return nullptr;
		}
		else
		{
			return iter->get();
		}
	}

	FORCEINLINE NODISCARD SceneManager& GetSceneManager() { return SceneManager::Get(); }
}