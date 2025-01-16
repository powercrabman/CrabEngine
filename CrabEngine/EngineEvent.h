#pragma once
#include "IEvent.h"

namespace crab
{
	class Scene;

	class ChangeScene_EngineEvent : public IEvent
	{
	public:
		EVENT_IMPLEMENT(ChangeScene_EngineEvent);
		std::string ToString() override { return s_staticName; }

		Scene* m_scene;
	};
}