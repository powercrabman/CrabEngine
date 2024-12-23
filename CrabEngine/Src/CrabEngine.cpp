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
		m_appName   = in_prop.applcationName;
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

			// Update
			SceneManager::Get().OnUpdate(dt);

			// Render
			if (m_editor)
			{
				_editor_rendering_loop_(dt);
			}
			else
			{
				_runtime_rendering_loop_(dt);
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
			MouseClickEvent e;
			e.m_mouseMode = (eMouse)in_event.button.button;
			e.m_x = in_event.button.x;
			e.m_y = in_event.button.y;
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

	void CrabEngine::_editor_rendering_loop_(float in_deltaTime)
	{
		Renderer::RenderBegin(); // 렌더링 시작
		m_editor->BindSceneRenderTarget(); // Scene 을 위한 렌더링 공간 마련
		Renderer::ClearBuffer(m_clearColor); // Clear

		SceneManager::Get().OnRender(in_deltaTime); // Scene Rendering

		ImGuiRenderer::RenderBegin(); // ImGui 렌더링
		SceneManager::Get().OnImGuiRender(in_deltaTime); // Scene ImGui

		Renderer::BindRenderTarget(nullptr); // 기본 렌더 타겟으로 변경
		Renderer::ClearBuffer(m_clearColor); // Clear
		m_editor->OnImGuiRender(in_deltaTime); // Editor ImGui
		ImGuiRenderer::RenderEnd(); // ImGui 렌더링 종료

		Renderer::RenderEnd(); // 렌더링 종료
		Renderer::Present(); // Bitblt
	}

	void CrabEngine::_runtime_rendering_loop_(float in_deltaTime) const
	{
		Renderer::RenderBegin(); // 렌더링 시작
		Renderer::BindRenderTarget(nullptr); // 기본 렌더 타겟 사용
		Renderer::ClearBuffer(m_clearColor); // 렌더 타겟 Clear

		SceneManager::Get().OnRender(in_deltaTime); // 씬 렌더링

		ImGuiRenderer::RenderBegin(); // ImGui 렌더링 시작
		SceneManager::Get().OnImGuiRender(in_deltaTime);
		ImGuiRenderer::RenderEnd(); // ImGui 렌더링 종료

		Renderer::RenderEnd(); // 렌더링 종료
		Renderer::Present(); // Bitblt
	}
}
