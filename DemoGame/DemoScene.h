#pragma once
#include "Scene.h"

class DemoScene : public crab::Scene
{

public:
	 DemoScene() = default;
	~DemoScene() override = default;

	void OnSetupScene() override;
	void OnEnterScene() override;
	void OnExitScene() override;

	void OnUpdate(crab::TimeStamp& in_ts) override;
	void OnRender(crab::TimeStamp& in_ts) override;
	void OnImGuiRender(crab::TimeStamp& in_ts) override;

	void OnEvent(crab::IEvent& in_event) override;

	REGISTER_SCENE(DemoScene);

private:
	crab::Entity e = crab::Entity::s_null;
	crab::Ref<crab::IShader> m_shader;

	int32 m_entityCount = 1;
};

