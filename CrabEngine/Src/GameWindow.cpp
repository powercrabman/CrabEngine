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

	bool GameWindow::Init(
		const std::string_view in_title /*= "cmEngine"*/,
		const int in_positionX /*= 0*/,
		const int in_positionY /*= 0*/,
		const int in_width /*= 800*/,
		const int in_height /*= 600 */
	)
	{
		const int positionX = (in_positionX == 0) ? SDL_WINDOWPOS_CENTERED : in_positionX;
		const int positionY = (in_positionY == 0) ? SDL_WINDOWPOS_CENTERED : in_positionY;

		m_window = SDL_CreateWindow(
			in_title.data(),
			positionX, positionY,
			in_width, in_height,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
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
}
