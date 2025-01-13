#pragma once
#include "CrabWindow.h"
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

		CrabWindow&			GetWindow() { return m_crabWindow; }
		const char*			GetApplicationName() const { return m_appName.c_str(); };

		bool				IsEnableEditor() const { return m_editor.get() ; }

	private:
		CrabEngine();
		~CrabEngine();

		bool		_init_(const ApplicationSetting& in_setting);

		int			_run_() const;
		void		_shutdown_(AppShutdown_Event& in_event) const;

		void		_on_event_(IEvent& in_event);

		Scope<Editor>	m_editor;
		CrabWindow		m_crabWindow;

		bool			m_isRunning = false;
		Vec4			m_clearColor = Color::BLACK;
		std::string		m_appName = {};
	};

	FORCEINLINE NODISCARD CrabEngine& GetCrabEngine() { return CrabEngine::Get(); }
}
