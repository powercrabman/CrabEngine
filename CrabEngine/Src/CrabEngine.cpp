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

	bool CrabEngine::_init_(const ApplicationProp& in_prop)
	{
		// SDL 초기화
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			ASSERT(false, "SDL_Init Error: {}", SDL_GetError());
			return false;
		}

		// GameWindow 생성
		if (!m_gameWindow.Init(
			in_prop.applcationName,
			in_prop.windowPositionX,
			in_prop.windowPositionY,
			in_prop.windowWidth,
			in_prop.windowHeight,
			in_prop.enableEditor
		))
		{
			ASSERT(false, "GameWindow init fail.");
			SDL_Quit();
			return false;
		}

		// Renderer 초기화
		Renderer::Init(in_prop.renderingAPI, in_prop.windowWidth, in_prop.windowHeight);

		// ImGui 초기화
		ImGuiRenderer::Init();

		// Client Asset 로딩
		ASSET_MANAGER.LoadClientAssets();

		// Editor
		if (in_prop.enableEditor)
		{
			m_editor = MakeScope<Editor>();
			m_editor->Init(in_prop.windowWidth, in_prop.windowHeight);
		}

		// Run Engine
		Log::Info("Engine initialize done.");
		m_isRunning = true;
		m_appName = in_prop.applcationName;
	}

	int CrabEngine::_run_()
	{
		Timer timer = {};
		SDL_Event event = {};

		while (m_isRunning)
		{
			// Application Event
			while (SDL_PollEvent(&event)) { m_gameWindow.TranslateEvent(event); }

			// Main Loop
			float dt = timer.CalcDeltaTime();

			if (m_editor)
			{
				// Loop in editing mode
				m_editor->OnUpdate(dt);
			}
			else
			{
				// Loop in runtime
				SceneManager::Get().OnUpdate(dt);

				Renderer::RenderBegin();
				{
					Renderer::BindRenderTarget(nullptr); // 기본 렌더타겟 사용
					Renderer::ClearBuffer(Color::BLACK);

					SceneManager::Get().OnRender(dt);

					ImGuiRenderer::RenderBegin();
					SceneManager::Get().OnImGuiRender(dt);
					ImGuiRenderer::RenderEnd();
				}
				Renderer::RenderEnd();
				Renderer::Present();
			}
		}

		return 0;
	}

	void CrabEngine::_shutdown_(AppShutdownEvent& in_event) const
	{
		ASSERT(!m_isRunning, "use have to dispatch AppCloseEvent to close application. do not use this event.");
		if (m_isRunning) { in_event.m_isHandled = true; }
	}


	void CrabEngine::DispatchEvent(IEvent& in_event)
	{
		Log::Debug("Event Occured => {}", in_event.ToString());

		_on_event_(in_event);
		Renderer::OnEvent(in_event);
		SceneManager::Get().OnEvent(in_event);
		ImGuiRenderer::OnEvent(in_event);
		ComponentSerializerManager::Get().OnEvent(in_event);
		if (m_editor) { m_editor->OnEvent(in_event); }
	}

	void CrabEngine::_on_event_(IEvent& in_event)
	{
		EventDispatcher dispatcher{ in_event };

		dispatcher.Dispatch<AppCloseEvent>([&](AppCloseEvent& in_event) { m_isRunning = false; });
		dispatcher.Dispatch<AppShutdownEvent>(BIND_FN_CALLBACK_ARG1(this, CrabEngine::_shutdown_));
	}
}
