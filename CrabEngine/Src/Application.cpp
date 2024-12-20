#include "CrabEnginePch.h"
#include "Application.h"
#include "CrabEngine.h"

namespace crab
{
	void Application::_init_(const ApplicationProp& in_prop)
	{
		// base initialize
		Log::Init();
		CrabEngine::Get()._init_(in_prop);

		// user initialize
		Initialize();
	}

	void Application::_shutdown_()
	{
		// user shutdown
		Shutdown();

		// Terminate Application
		AppShutdownEvent e = {};
		CrabEngine::Get().DispatchEvent(e);

		// Destroy Engine
		CrabEngine::Destroy();
		Log::Shutdown();
	}

	int Application::_run_()
	{
		int result = CrabEngine::Get()._run_();
		_shutdown_();

		return result;
	}
}
