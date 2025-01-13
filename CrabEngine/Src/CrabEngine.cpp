#include "CrabEnginePch.h"
#include "CrabEngine.h"
#include "Timer.h"
#include "SceneManager.h"
#include "ImGuiRenderer.h"
#include "Editor.h"
#include "ComponentSerializerManager.h"

namespace crab
{
	CrabEngine::CrabEngine() {}
	CrabEngine::~CrabEngine() { SDL_Quit(); }

	bool CrabEngine::_init_(const ApplicationSetting& in_setting)
	{
		// CrabWindow 생성
		if (!m_crabWindow.Init(in_setting))
		{
			ASSERT(false, "CrabWindow init fail.");
			return false;
		}

		// Client Asset 초기화
		GetAssetManager().Init(in_setting);

		// Renderer 초기화
		Renderer::Init(in_setting);

		// ImGui 초기화
		ImGuiRenderer::Init(in_setting);

		// Script Engine 초기화
		MonoScriptEngine::Init(in_setting);

		// Assets Load
		GetAssetManager().LoadClientAssets();

		// Editor
		if (in_setting.enableEditor)
		{
			m_editor = MakeScope<Editor>();
			m_editor->Init(in_setting);
		}

		// Run Engine
		Log::Info("Engine initialize done.");

		m_isRunning = true;
		m_appName = in_setting.applicationName;

		return true;
	}

	int CrabEngine::_run_() const
	{
		SDL_Event event = {};

		const SceneManager& scMgr = GetSceneManager();

		Timer timer = {};
		TimeStamp timeStamp = {};

		while (m_isRunning)
		{
			// Application Event
			while (SDL_PollEvent(&event)) { m_crabWindow.TranslateEvent(event); }

			// Time Check
			timer.CalcTimeStamp(timeStamp);

			// Main Loop
			if (m_editor)
			{
				// Loop in editor
				m_editor->OnUpdate(timeStamp);
			}
			else
			{
				// Loop in application
				scMgr.OnUpdate(timeStamp);

				Renderer::BeginRender();
				{
					Renderer::ResetRenderFrame();
					Renderer::ClearRenderFrame();

					scMgr.OnRender(timeStamp);

					ImGuiRenderer::BeginRender();
					scMgr.OnImGuiRender(timeStamp);
					ImGuiRenderer::EndRender();
				}
				Renderer::EndRender();
				Renderer::Present();
			}
		}

		return 0;
	}

	void CrabEngine::_shutdown_(AppShutdown_Event& in_event) const
	{
		ASSERT(!m_isRunning, "use have to dispatch AppClose_Event to close application. do not use this event.");
		if (m_isRunning)
		{
			in_event.m_isHandled = true;
			return;
		}
		else
		{
			ComponentSerializerManager::Shutdown();
			ImGuiRenderer::Shutdown();
			MonoScriptEngine::Shutdown();
			GetAssetManager().Shutdown();
		}
	}

	void CrabEngine::DispatchEvent(IEvent& in_event)
	{
		Log::Debug("Event Occured : {}", in_event.ToString());

		_on_event_(in_event);
		Renderer::OnEvent(in_event);
		GetSceneManager().OnEvent(in_event);
		if (m_editor) { m_editor->OnEvent(in_event); }
	}

	void CrabEngine::_on_event_(IEvent& in_event)
	{
		EventDispatcher dispatcher{ in_event };

		dispatcher.Dispatch<AppClose_Event>([&](AppClose_Event& in_event) { m_isRunning = false; });
		dispatcher.Dispatch<AppShutdown_Event>([&](AppShutdown_Event& in_event) { CrabEngine::_shutdown_(in_event); });
	}
}
