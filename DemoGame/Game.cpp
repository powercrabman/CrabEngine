#include "CrabEnginePch.h"
#include "Application.h"
#include "DemoScene.h"
#include "MonoTestScene.h"
#include "Physic2D.h"

class DemoGame : public crab::Application
{
public:
	crab::ApplicationSetting SetupApplication(const int in_argc, char* const* in_argv) override
	{
		crab::ApplicationSetting setting = {};
		setting.applicationName          = "DemoGame";
		setting.renderingAPI             = crab::eRenderAPI::DirectX11;
		setting.windowWidth              = 1920;
		setting.windowHeight             = 1080;
		setting.windowPositionX          = 100;
		setting.windowPositionY          = 50;
		setting.enableEditor             = true;

		setting.scriptEngineDLLPath = R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabEngine\CrabScriptCore\bin\Debug\CrabScriptCore.dll)";
		setting.projectScriptDLLPath = R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabEngine\DemoGameScript\bin\Debug\DemoGameScript.dll)";
		setting.engineDirectory = R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabEngine\CrabEngine)";
		setting.projectDirectory = R"(C:\Users\Ahnjiwoo\Desktop\Projects\CrabEngine\DemoGame)";

		crab::GetSceneManager().EmplacePlugin<crab::Physic2D>();

		return setting;
	}

	void Initialize() override 
	{
		crab::GetSceneManager().ChangeScene<MonoTestScene>();
	}

	void Shutdown() override {}
};

CRAB_MAIN(DemoGame)