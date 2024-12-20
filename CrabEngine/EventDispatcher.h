#pragma once

#include "Event.h"

namespace crab
{
	class EventDispatcher
	{
	public:
		EventDispatcher(IEvent& event) : m_event(event) {}

		template<typename Ty, typename Func>
		bool Dispatch(const Func& in_func) 
		{
			if (m_event.GetType() == Ty::StaticType && !m_event.m_isHandled)
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