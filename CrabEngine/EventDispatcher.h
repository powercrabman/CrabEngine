#pragma once
#include "CrabEvent.h"

#define CRAB_REGISTER_EVENT_HANDLER(eventType, HandlerCallback)\
		if(dispatcher.Dispatch<eventType>(HandlerCallback)) return\

namespace crab
{
	class EventDispatcher
	{
	public:
		EventDispatcher(IEvent& event) : m_event(event) {}

		template<typename Ty, typename Func>
		bool Dispatch(const Func& in_func) 
		{
			if (m_event.GetType() == Ty::s_staticType && !m_event.m_isHandled)
			{
				in_func(static_cast<Ty&>(m_event));
				return true;
			}
			return false;
		}

	private:
		IEvent& m_event;
	};
}