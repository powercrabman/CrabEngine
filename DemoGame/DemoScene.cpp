#include "pch.h"
#include "DemoScene.h"
#include <AssetSerializer.h>
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteBatch.h"

void DemoScene::OnSetupScene()
{
	using namespace crab;
}

void DemoScene::OnEnterScene()
{
	using namespace crab;

	Renderer::BindDepthStencilState(eDepthStencilState::DepthDefault);
	Renderer::BindSamplerState(eSamplerState::PointWrap, 0);
}

void DemoScene::OnExitScene()
{
}

void DemoScene::OnUpdate(crab::TimeStamp& in_ts)
{
}

void DemoScene::OnRender(crab::TimeStamp& in_ts)
{
}

void DemoScene::OnImGuiRender(crab::TimeStamp& in_ts)
{
}

void DemoScene::OnEvent(crab::IEvent& in_event)
{
}

