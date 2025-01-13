#pragma once
#include "Application.h"

#define CRAB_MAIN(ApplicationType)										\
int main(int argc, char* argv[])										\
{																		\
	ApplicationType app;												\
	crab::ApplicationSetting prop = app.SetupApplication(argc, argv);	\
	app._init_(prop);													\
	return app._run_();													\
}