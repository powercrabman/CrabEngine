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

		void		_on_event_(IEvent& in_event);

		Scope<Editor>	m_editor;
		GameWindow		m_gameWindow;

		bool			m_isRunning = false;
		Vec4			m_clearColor = Color::BLACK;
		std::string		m_appName = {};
	};
}
