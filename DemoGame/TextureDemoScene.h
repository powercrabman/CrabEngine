#pragma once
#include "Scene.h"

class TextureDemoScene : public crab::Scene
{
public:
	TextureDemoScene() = default;
	~TextureDemoScene() = default;

	void OnEnterScene() override;
	void OnExitScene() override;

	void OnUpdate(float in_deltaTime) override;
	void OnRender(float in_deltaTime) override;
	void OnImGuiRender(float in_deltaTime) override;

	REGIST_SCENE(TextureDemoScene);

public:
	crab::Ref<crab::Geometry> m_geometry;
	crab::Ref<crab::ITexture> m_texture;

	Vec3 m_position = Vec3::Zero;
	Vec3 m_rot      = Vec3::Zero;
	Vec3 m_scale    = Vec3::One;
};

