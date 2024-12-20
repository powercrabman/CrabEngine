#pragma once
#include "Scene.h"


class GameScene : public crab::Scene
{
public:
	GameScene() = default;
	~GameScene() = default;

	void OnEnterScene()						override;
	void OnExitScene()						override;

	void OnUpdate(float in_deltaTime)		override;
	void OnRender(float in_deltaTime)		override;
	void OnImGuiRender(float in_deltaTime)	override;

	REGIST_SCENE(GameScene);

private:
	crab::Ref<crab::Geometry> m_geometry;
};

