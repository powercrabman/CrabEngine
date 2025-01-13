#include "CrabEnginePch.h"
#include "CrabWindow.h"

namespace crab
{
	CrabWindow::~CrabWindow()
	{
		if (m_window)
		{
			SDL_DestroyWindow(m_window);
		}
		SDL_Quit();
	}

	void CrabWindow::TranslateEvent(const SDL_Event& in_event) const
	{
		ImGui_ImplSDL2_ProcessEvent(&in_event);

		switch (in_event.type)
		{

		case SDL_QUIT:
		{
			AppClose_Event e;
			GetCrabEngine().DispatchEvent(e);
			break;
		}

		case SDL_WINDOWEVENT:
		{
			_handle_window_event_(in_event);
			break;
		}

		break;
		}
	}

	bool CrabWindow::Init(const ApplicationSetting& in_setting)
	{
		// SDL 초기화
		if (SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			ASSERT(false, "SDL_Init Error: {}", SDL_GetError());
			return false;
		}

		const int positionX = (in_setting.windowPositionX == 0) ? SDL_WINDOWPOS_CENTERED : in_setting.windowPositionX;
		const int positionY = (in_setting.windowPositionY == 0) ? SDL_WINDOWPOS_CENTERED : in_setting.windowPositionY;

		Uint32 flag = SDL_WINDOW_SHOWN;
		flag |= in_setting.enableEditor ? SDL_WINDOW_RESIZABLE : NULL;

		m_window = SDL_CreateWindow(
			in_setting.applicationName.c_str(),
			positionX, positionY,
			in_setting.windowWidth, in_setting.windowHeight,
			flag
		);

		if (!m_window)
		{
			ASSERT(false, "SDL_CreateWindow Error: {}", SDL_GetError());
			return false;
		}

		return true;
	}

	void* CrabWindow::GetNativeWindow() const
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

	void CrabWindow::_handle_window_event_(const SDL_Event& in_event) const
	{
		switch (in_event.window.event)
		{
		case SDL_WINDOWEVENT_RESIZED:
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			if (in_event.window.windowID == SDL_GetWindowID(m_window))
			{
				WindowResize_Event e;
				e.m_width = in_event.window.data1;
				e.m_height = in_event.window.data2;
				GetCrabEngine().DispatchEvent(e);
			}
			break;

		case SDL_WINDOWEVENT_MOVED:
			if (in_event.window.windowID == SDL_GetWindowID(m_window))
			{
				WindowMove_Event e;
				e.m_x = in_event.window.data1;
				e.m_y = in_event.window.data2;
				GetCrabEngine().DispatchEvent(e);
			}
			break;

		case SDL_WINDOWEVENT_CLOSE:
			{
				AppClose_Event e;
				GetCrabEngine().DispatchEvent(e);
			}
			break;
		}
	}

}
