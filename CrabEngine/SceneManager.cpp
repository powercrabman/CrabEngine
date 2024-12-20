#include "CrabEnginePch.h"
#include "SceneManager.h"
#include "Scene.h"

namespace crab
{
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

}