#pragma once
#include "GameWindow.h"
#include "Singleton.h"
#include "Application.h"
#include "BatchEventProcessor.h"

namespace crab
{
	class Editor;

	class CrabEngine : public Singleton<CrabEngine>
	{
		friend class Application;
		friend class Singleton<CrabEngine>;

	public:
		template <typename EventType>
		void		DispatchDelayedEvent(EventType& in_event);
		void		DispatchEvent(IEvent& in_event);

		GameWindow&	GetWindow() { return m_crabWindow; }
		const char*	GetApplicationName() const { return m_appName.c_str(); };

		bool		IsEnableEditor() const { return m_editor.get() ; }

	private:
		 CrabEngine();
		~CrabEngine();

		bool init_engine(const ApplicationSetting& in_setting);
		void late_init_engine(const ApplicationSetting& in_setting);
		int	 run_engine();
		void shutdown_engine(AppShutdown_Event& in_event) const;
		void on_event(IEvent& in_event);

		Scope<Editor>		m_editor;
		GameWindow			m_crabWindow;
		BatchEventProcessor m_batchEventProcessor;

		bool				m_isRunning = false;
		Vec4				m_clearColor = Color::BLACK;
		std::string			m_appName = {};
	};

	FORCEINLINE NODISCARD CrabEngine& GetEngine() { return CrabEngine::Get(); }

	template<typename EventType>
	inline void CrabEngine::DispatchDelayedEvent(EventType& in_event)
	{
		static_assert(IS_BASE_OF(IEvent, EventType));
		m_batchEventProcessor.EmplaceEvent<EventType>(in_event);
	}
}
