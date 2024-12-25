#include "CrabEnginePch.h"
#include "CrabEngine.h"
#include "Timer.h"
#include "SceneManager.h"
#include "ImGuiRenderer.h"
#include "Editor.h"

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
			while (SDL_PollEvent(&event)) { _translate_event_(event); }

			// Main Loop
			Input::UpdateInputState();
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

	void CrabEngine::_translate_event_(const SDL_Event& in_event)
	{
		ImGui_ImplSDL2_ProcessEvent(&in_event);

		switch (in_event.type)
		{

		case SDL_QUIT:
		{
			AppCloseEvent e;
			DispatchEvent(e);
			break;
		}

		case SDL_WINDOWEVENT:
		{
			_handle_window_event_(in_event);
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			MousePressEvent e;
			e.m_mouseMode = (eMouse)in_event.button.button;
			e.m_x = in_event.button.x;
			e.m_y = in_event.button.y;
			DispatchEvent(e);
			break;
		}

		case SDL_MOUSEWHEEL:
		{
			MouseWheelEvent e;
			e.m_dx = in_event.wheel.preciseX;
			e.m_dy = in_event.wheel.preciseY;
			DispatchEvent(e);
			break;
		}

		}
	}

	void CrabEngine::_handle_window_event_(const SDL_Event& in_event)
	{
		switch (in_event.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
		{
			if (in_event.window.windowID == SDL_GetWindowID(m_gameWindow.GetSDLWindow()))
			{
				WindowResizeEvent e;
				e.m_width = in_event.window.data1;
				e.m_height = in_event.window.data2;
				DispatchEvent(e);
			}
			break;
		}
		}
	}

	void CrabEngine::DispatchEvent(IEvent& in_event)
	{
		Log::Debug("Event Occured => {}", in_event.ToString());

		_on_event_(in_event);
		Renderer::OnEvent(in_event);
		SceneManager::Get().OnEvent(in_event);
		ImGuiRenderer::OnEvent(in_event);
		if (m_editor) { m_editor->OnEvent(in_event); }
	}

	void CrabEngine::_on_event_(IEvent& in_event)
	{
		EventDispatcher dispatcher{ in_event };

		dispatcher.Dispatch<AppCloseEvent>([&](AppCloseEvent& in_event) { m_isRunning = false; });
		dispatcher.Dispatch<AppShutdownEvent>(BIND_FN_CALLBACK_ARG1(this, CrabEngine::_shutdown_));
	}
}
