#pragma once
#include "EntryPoint.h"

namespace crab
{
	struct ApplicationSetting
	{
		// Application
		std::string applicationName = "cmEngine";

		int windowPositionX = 0;
		int windowPositionY = 0;

		int windowWidth  = 800;
		int windowHeight = 600;

		// Directory
		std::string engineDirectory;
		std::string projectDirectory;

		std::string scriptEngineDLLPath;
		std::string projectScriptDLLPath;

		// Rendering
		eRenderAPI	renderingAPI = eRenderAPI::DirectX11;

		// Editor
		bool		enableEditor = false;
	};

	class Application
	{
		friend int ::main(int argc, char* argv[]);

	public:
		 		 Application();
		virtual ~Application();

		virtual void Initialize() = 0;
		virtual void Shutdown()   = 0;

	private:
		void	init_app(const ApplicationSetting& in_setting = ApplicationSetting{});
		void	shutdown_app();
		int		run_app();

		virtual crab::ApplicationSetting	SetupApplication(const int in_argc, char* const* in_argv) = 0;
	};

}

