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
			plugin->OnDetach();
		}

		SceneManager::Destroy();
	}

	Scene* SceneManager::TryFindSceneByName(const std::string_view in_name)
	{
		if (in_name.empty()) return nullptr;

		auto iter = std::find_if(m_sceneRepo.begin(), m_sceneRepo.end(), [&](const auto& in_data)
			{
				return in_data.second->GetName() == in_name;
			}
		);

		return iter->second.get();
	}

	crab::Scene* SceneManager::TryGetCurrentScene() const
	{
		return m_currentScene;
	}

	void SceneManager::ChangeScene(Scene* in_scene) 
	{
		if (in_scene) 
		{
			ChangeScene_EngineEvent event;
			event.m_scene = in_scene;
			GetEngine().DispatchDelayedEvent(event);
		} 
		else
		{
			assert(false);
		}
	}

	void SceneManager::RestartCurrentScene()
	{
		if (m_currentScene)
		{
			ChangeScene(m_currentScene);
		}
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

	void SceneManager::OnEvent(IEvent& in_event)
	{
		EventDispatcher dispatcher{ in_event };

		CRAB_REGISTER_EVENT_HANDLER(AppShutdown_Event,
			[&](AppShutdown_Event& in_event)
			{
				Shutdown();
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(ChangeScene_EngineEvent,
			[&](ChangeScene_EngineEvent& in_event)
			{
				if (in_event.m_scene)
				{
					change_scene(in_event.m_scene);
				}
				else
				{
					Log::Error("Can not Find Scene : {}", in_event.m_scene->GetName());
				}
			}
		);

		if (m_currentScene) { m_currentScene->OnEventSuper(in_event); }
	}

	void SceneManager::change_scene(Scene* in_scene)
	{
		if (in_scene)
		{
			Log::Info("Change scene to {}", in_scene->GetName());

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