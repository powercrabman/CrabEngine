#include "CrabEnginePch.h"
#include "Application.h"
#include "CrabEngine.h"

namespace crab
{
	Application::Application()
	{
		Log::Init();
	}

	Application::~Application()
	{
		Log::Shutdown();
	}

	void Application::init_app(const ApplicationSetting& in_setting)
	{
		// base initialize
		GetEngine().init_engine(in_setting);

		// user initialize
		Initialize();

		// late initialize
		GetEngine().late_init_engine(in_setting);
	}

	void Application::shutdown_app()
	{
		// user shutdown
		Shutdown();

		// Terminate Application
		AppShutdown_Event e = {};
		GetEngine().DispatchEvent(e);

		// Destroy Engine
		CrabEngine::Destroy();
	}

	int Application::run_app()
	{
		int result = CrabEngine::Get().run_engine();
		shutdown_app();

		return result;
	}
}
