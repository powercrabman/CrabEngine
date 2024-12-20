#pragma once
#include "GameWindow.h"
#include "Singleton.h"
#include "Application.h"

namespace crab
{
	class CrabEngine : public Singleton<CrabEngine>
	{
		friend class Application;
		friend class Singleton<CrabEngine>;

	public:
		GameWindow& GetWindow() { return m_gameWindow; }
		void		DispatchEvent(IEvent& in_event);

	private:
		CrabEngine();
		~CrabEngine();

		bool		_init_(const ApplicationProp& in_prop);

		int			_run_();
		void		_shutdown_(AppShutdownEvent& in_event) const;

		void		_translate_event_(const SDL_Event& in_event);
		void		_handle_window_event_(const SDL_Event& in_event);
		void		_on_event_(IEvent& in_event);

		GameWindow	m_gameWindow;
		bool m_isRunning = false;
	};
}
