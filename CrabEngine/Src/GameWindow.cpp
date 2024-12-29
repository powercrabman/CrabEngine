#include "CrabEnginePch.h"
#include "GameWindow.h"

namespace crab
{
	GameWindow::~GameWindow()
	{
		if (m_window)
		{
			SDL_DestroyWindow(m_window);
		}
	}

	void GameWindow::TranslateEvent(const SDL_Event& in_event)
	{
		ImGui_ImplSDL2_ProcessEvent(&in_event);

		switch (in_event.type)
		{

		case SDL_QUIT:
		{
			AppCloseEvent e;
			CRAB_ENGINE.DispatchEvent(e);
			break;
		}

		case SDL_WINDOWEVENT:
		{
			_handle_window_event_(in_event);
			break;
		}
		}
	}

	bool GameWindow::Init(
		const std::string_view	in_title,
		const int				in_positionX,
		const int				in_positionY,
		const int				in_width,
		const int				in_height,
		const bool				in_useCustomTitlebar
	)
	{
		// 입력에 따른 처리
		const int positionX = (in_positionX == 0) ? SDL_WINDOWPOS_CENTERED : in_positionX;
		const int positionY = (in_positionY == 0) ? SDL_WINDOWPOS_CENTERED : in_positionY;

		Uint32 flag = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

		m_window = SDL_CreateWindow(
			in_title.data(),
			positionX, positionY,
			in_width, in_height,
			flag
		);

		if (!m_window)
		{
			ASSERT(false, "SDL_CreateWindow Error: {}", SDL_GetError());
			return false;
		}

		return true;
	}

	void* GameWindow::GetNativeWindow() const
	{
		SDL_SysWMinfo wmInfo = {};
		SDL_VERSION(&wmInfo.version);

		ASSERT(SDL_GetWindowWMInfo(m_window, &wmInfo), "SDL_GetWindowWMInfo failed");

#ifdef _WIN32
		return wmInfo.info.win.window;
#elif __APPLE__
		return wmInfo.info.cocoa.window;
#elif __linux__
		return reinterpret_cast<void*>(wmInfo.info.x11.window);
#elif __ANDROID__
		return wmInfo.info.android.window;
#elif SDL_SYSWM_WAYLAND
		return wmInfo.info.wl.surface;
#else
		ASSERT(false, "Unsupported platform");
		return nullptr;
#endif
	}

	void GameWindow::_handle_window_event_(const SDL_Event& in_event)
	{
		switch (in_event.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			if (in_event.window.windowID == SDL_GetWindowID(m_window))
			{
				WindowResizeEvent e;
				e.m_width = in_event.window.data1;
				e.m_height = in_event.window.data2;
				CRAB_ENGINE.DispatchEvent(e);
			}
			break;

		case SDL_WINDOWEVENT_MOVED:
			if (in_event.window.windowID == SDL_GetWindowID(m_window))
			{
				WindowMoveEvent e;
				e.m_x = in_event.window.data1;
				e.m_y = in_event.window.data2;
				CRAB_ENGINE.DispatchEvent(e);
			}
			break;

		case SDL_WINDOWEVENT_CLOSE:
			{
				AppCloseEvent e;
				CRAB_ENGINE.DispatchEvent(e);
			}
			break;
		}
	}

}
