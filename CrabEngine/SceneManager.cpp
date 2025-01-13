#include "CrabEnginePch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneSerializer.h"
#include "IPlugin.h"

namespace crab
{
	void SceneManager::Shutdown() const
	{
		for (const auto& plugin : m_plugins)
		{
			plugin->Shutdown();
		}

		SceneManager::Destroy();
	}

	Scene* SceneManager::TryFindSceneByName(const std::string_view in_name)
	{
		auto iter = std::find_if(m_sceneRepo.begin(), m_sceneRepo.end(), [&](const auto& in_data) 
			{
				return in_data.second->ToString() == in_name;
			}
		);

		assert(iter != m_sceneRepo.end());
		return iter->second.get();
	}

	crab::Scene* SceneManager::TryGetCurrentScene() const
	{
		return m_currentScene;
	}

	void SceneManager::OnUpdate(TimeStamp& in_ts) const
	{
		if (m_currentScene)
		{
			m_currentScene->OnUpdateSuper(in_ts);

			for (const auto& plugin : m_plugins) plugin->OnUpdateScene(in_ts);
		}
	}

	void SceneManager::OnRender(TimeStamp& in_ts) const
	{
		if (m_currentScene)
		{
			m_currentScene->OnRenderSuper(in_ts);

			for (const auto& plugin : m_plugins) plugin->OnRenderScene(in_ts);
		}
	}

	void SceneManager::OnImGuiRender(TimeStamp& in_ts) const
	{
		if (m_currentScene)
		{
			m_currentScene->OnImGuiRender(in_ts);
		}
	}

	void SceneManager::OnEvent(IEvent& in_event) const
	{
		EventDispatcher dispatcher{ in_event };

		if (dispatcher.Dispatch<AppShutdown_Event>([&](AppShutdown_Event& in_event) { Shutdown(); }))
		{
			return;
		}

		if (m_currentScene)
		{
			m_currentScene->OnEventSuper(in_event);
		}
	}

	void SceneManager::_change_scene_(Scene* in_scene)
	{
		if (in_scene)
		{
			if (m_currentScene)
			{
				for (const auto& plugin : m_plugins) plugin->OnExitScene();
				m_currentScene->OnExitSceneSuper();
			}

			m_currentScene = in_scene;
			m_currentScene->OnEnterSceneSuper();
			for (const auto& plugin : m_plugins) plugin->OnEnterScene();
		}
	}

}