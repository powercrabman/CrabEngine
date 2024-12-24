#include "CrabEnginePch.h"
#include "Application.h"
#include "GameScene.h"
#include "TextureDemoScene.h"
#include "EntityAssetDemoScene.h"

class DemoGame : public crab::Application
{
public:
	crab::ApplicationProp SetupApplication(const int in_argc, char* const* in_argv) override
	{
		crab::ApplicationProp prop = {};
		prop.applcationName        = "DemoGame";
		prop.renderingAPI          = crab::eRenderAPI::DirectX11;
		prop.windowWidth           = 1920;
		prop.windowHeight          = 1080;
		prop.windowPositionX       = 100;
		prop.windowPositionY       = 50;
		prop.enableEditor          = true;

		return prop;
	}

	void Initialize() override 
	{
		crab::SceneManager::Get().ChangeScene<EntityAssetDemoScene>();
	}

	void Shutdown() override {}
};

CRAB_MAIN(DemoGame)