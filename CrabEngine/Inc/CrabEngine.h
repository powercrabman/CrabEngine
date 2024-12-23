#pragma once
#include "GameWindow.h"
#include "Singleton.h"
#include "Application.h"

namespace crab
{
	class Editor;

	class CrabEngine : public Singleton<CrabEngine>
	{
		friend class Application;
		friend class Singleton<CrabEngine>;

	public:
		void				DispatchEvent(IEvent& in_event);

		GameWindow&			GetWindow() { return m_gameWindow; }
		std::string_view	GetApplicationName() const { return m_appName; };

	private:
		CrabEngine();
		~CrabEngine();

		bool		_init_(const ApplicationProp& in_prop);

		int			_run_();
		void		_shutdown_(AppShutdownEvent& in_event) const;

		void		_translate_event_(const SDL_Event& in_event);
		void		_handle_window_event_(const SDL_Event& in_event);
		void		_on_event_(IEvent& in_event);

		void		_editor_rendering_loop_(float in_deltaTime);
		void		_runtime_rendering_loop_(float in_deltaTime) const;

		GameWindow		m_gameWindow;
		Scope<Editor>	m_editor;

		bool			m_isRunning = false;
		Vec4			m_clearColor = Color::BLACK;
		std::string		m_appName = {};
	};
}
