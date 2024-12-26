#include "CrabEnginePch.h"
#include "SceneManager.h"
#include "Scene.h"

namespace crab
{

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

	void SceneManager::OnUpdate(float in_deltaTime)
	{
		if (m_currentScene)
		{
			m_currentScene->OnUpdate(in_deltaTime);
		}
	}

	void SceneManager::OnRender(float in_deltaTime)
	{
		if (m_currentScene)
		{
			m_currentScene->OnRender(in_deltaTime);
		}
	}

	void SceneManager::OnImGuiRender(float in_deltaTime)
	{
		if (m_currentScene)
		{
			m_currentScene->OnImGuiRender(in_deltaTime);
		}
	}

	void SceneManager::OnEvent(IEvent& in_event)
	{
		EventDispatcher dispatcher{ in_event };

		dispatcher.Dispatch<AppShutdownEvent>([](AppShutdownEvent& in_event) { SceneManager::Destroy(); });
	}

	void SceneManager::_change_scene_(Scene* in_scene)
	{
		if (in_scene)
		{
			if (m_currentScene) { m_currentScene->OnExitScene(); }

			m_currentScene = in_scene;

			// Scene Resource Setup
			if (!m_currentScene->m_isSetuped) 
			{
				m_currentScene->SetupScene(); 
				m_currentScene->m_isSetuped = true;
			}

			// Enter
			m_currentScene->OnEnterScene();
		}
	}

}