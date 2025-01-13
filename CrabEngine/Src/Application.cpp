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

	void Application::_init_(const ApplicationSetting& in_setting)
	{
		// base initialize
		GetCrabEngine()._init_(in_setting);

		// user initialize
		Initialize();
	}

	void Application::_shutdown_()
	{
		// user shutdown
		Shutdown();

		// Terminate Application
		AppShutdown_Event e = {};
		GetCrabEngine().DispatchEvent(e);

		// Destroy Engine
		CrabEngine::Destroy();
	}

	int Application::_run_()
	{
		int result = CrabEngine::Get()._run_();
		_shutdown_();

		return result;
	}
}
