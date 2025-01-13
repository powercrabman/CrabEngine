#pragma once
#include "Timer.h"

class MonoTestScene : public crab::Scene
{
public:
	 MonoTestScene() = default;
	~MonoTestScene() override = default;

	void OnSetupScene() override;	
	void OnEnterScene() override;	
	void OnExitScene() override;

	void OnUpdate(crab::TimeStamp& in_ts) override;
	void OnRender(crab::TimeStamp& in_ts) override;
	void OnImGuiRender(crab::TimeStamp& in_ts) override;

	void OnEvent(crab::IEvent& in_event) override;

	REGISTER_SCENE(MonoTestScene);
};

