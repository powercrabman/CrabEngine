#pragma once

#include <SDL/include/SDL_syswm.h>
class SDL_Window;

namespace crab
{
	class GameWindow
	{
	public:
		GameWindow() = default;
		~GameWindow();

		/* position 을 명시적으로 설정하지 않을 경우 윈도우 중앙 정렬*/
		bool Init(
			const std::string_view in_title = "cmEngine",
			const int in_positionX = 0,
			const int in_positionY = 0,
			const int in_width = 800,
			const int in_height = 600
		);

		void ResizeWindow(const int in_width, const int in_height)
		{
			SDL_SetWindowSize(m_window, in_width, in_height);
		}

		void MoveWindow(const int in_positionX, const int in_positionY)
		{
			SDL_SetWindowPosition(m_window, in_positionX, in_positionY);
		}

		void SetWindowitle(const char* in_title)
		{
			SDL_SetWindowTitle(m_window, in_title);
		}

		auto GetWindowSize() const
		{
			int w, h;
			SDL_GetWindowSize(m_window, &w, &h);
			return std::make_pair(w, h);
		}

		auto GetWindowPosition() const
		{
			int x, y;
			SDL_GetWindowPosition(m_window, &x, &y);
			return std::make_pair(x, y);
		}

		SDL_Window* GetSDLWindow() const { return m_window; }
		void*		GetNativeWindow() const;

	private:
		SDL_Window* m_window = nullptr;
	};
}