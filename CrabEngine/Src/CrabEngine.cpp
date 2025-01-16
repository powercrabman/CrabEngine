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

	bool CrabEngine::init_engine(const ApplicationSetting& in_setting)
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

		// Event Processor
		m_batchEventProcessor.SetProcessorCallback([&](IEvent& in_event) { DispatchEvent(in_event); });

		// value setting
		m_appName = in_setting.applicationName;

		return true;
	}

	void CrabEngine::late_init_engine(const ApplicationSetting& in_setting)
	{
		// Editor
		if (in_setting.enableEditor)
		{
			m_editor = MakeScope<Editor>();
			m_editor->Init(in_setting);
		}

		// Initialize Done
		m_isRunning = true;
		Log::Info("Engine initialize done.");
	}

	int CrabEngine::run_engine()
	{
		SDL_Event event = {};
		const SceneManager& scMgr = GetSceneManager();
		Timer timer = {};
		TimeStamp timeStamp = {};

		while (m_isRunning)
		{
			// Application Event
			while (SDL_PollEvent(&event)) { m_crabWindow.ProcessEvent(event); }

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

				// rendering begin
				Renderer::BeginRender();
				Renderer::ResetRenderFrame();
				Renderer::ClearRenderFrame();

				// main game rendering
				Draw2D::BeginBatch();
				scMgr.OnRender(timeStamp);
				Draw2D::EndBatch();
				
				// gui rendering
				ImGuiRenderer::BeginRender();
				scMgr.OnImGuiRender(timeStamp);
				ImGuiRenderer::EndRender();

				// rendering end
				Renderer::EndRender();
				Renderer::Present();
			}

			// Delay Event Process
			m_batchEventProcessor.ProcessEvent();
		}

		return 0;
	}

	void CrabEngine::shutdown_engine(AppShutdown_Event& in_event) const
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

		on_event(in_event);
		Renderer::OnEvent(in_event);
		GetSceneManager().OnEvent(in_event);
		if (m_editor) { m_editor->OnEvent(in_event); }
	}

	void CrabEngine::on_event(IEvent& in_event)
	{
		EventDispatcher dispatcher{ in_event };

		CRAB_REGISTER_EVENT_HANDLER(AppClose_Event,
			[&](AppClose_Event& in_event)
			{
				m_isRunning = false;
			}
		);

		CRAB_REGISTER_EVENT_HANDLER(AppShutdown_Event,
			[&](AppShutdown_Event& in_event)
			{
				CrabEngine::shutdown_engine(in_event);
			}
		);
	}
}
