#pragma once
#include "EntryPoint.h"

namespace crab
{
	struct ApplicationProp
	{
		std::string applcationName = "cmEngine";

		int windowPositionX = 0;
		int windowPositionY = 0;

		int windowWidth = 800;
		int windowHeight = 600;

		eRenderAPI renderingAPI = eRenderAPI::OpenGL;
	};

	class Application
	{
		friend int ::main(int argc, char* argv[]);

	public:
		Application() = default;
		virtual ~Application() = default;

		virtual void Initialize() = 0;
		virtual void Shutdown()   = 0;

	private:
		void	_init_(const ApplicationProp& in_prop = ApplicationProp{});
		void	_shutdown_();
		int		_run_();

		virtual crab::ApplicationProp	SetupApplication(const int in_argc, char* const* in_argv) = 0;
	};

}

