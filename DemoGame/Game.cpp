#include "CrabEnginePch.h"
#include "Application.h"
#include "GameScene.h"
#include "TextureDemoScene.h"

class DemoGame : public crab::Application
{
public:
	crab::ApplicationProp SetupApplication(const int in_argc, char* const* in_argv) override
	{
		crab::ApplicationProp prop = {};
		prop.applcationName = "DemoGame";
		prop.renderingAPI = crab::eRenderAPI::DirectX11;
		prop.windowWidth = 1280;
		prop.windowHeight = 720;
		prop.windowPositionX = 100;
		prop.windowPositionY = 50;

		return prop;
	}

	void Initialize() override 
	{
		crab::SceneManager::Get().ChangeScene<TextureDemoScene>();
	}

	void Shutdown() override {}
};

CRAB_MAIN(DemoGame)