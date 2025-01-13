#pragma once

#include <SDL/include/SDL_syswm.h>

#include "Application.h"
class SDL_Window;

namespace crab
{
	class CrabWindow
	{
	public:
		CrabWindow() = default;
		~CrabWindow();

		void TranslateEvent(const SDL_Event& in_event) const;

		/* position 을 명시적으로 설정하지 않을 경우 윈도우 중앙 정렬*/
		bool Init(const ApplicationSetting& in_setting);

		void ResizeWindow(const int in_width, const int in_height) const
		{
			SDL_SetWindowSize(m_window, in_width, in_height);
		}

		void MoveWindow(const int in_positionX, const int in_positionY) const
		{
			SDL_SetWindowPosition(m_window, in_positionX, in_positionY);
		}

		void SetWindowTitle(const char* in_title) const
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
		void* GetNativeWindow() const;

	private:
		void _handle_window_event_(const SDL_Event& in_event) const;

		SDL_Window* m_window = nullptr;
	};
}