#pragma once

#include "Log.h"
#include "PlatformDetection.h"
#include "App.h"

#include <iostream>

extern Sprout::App* Sprout::CreateApp();

#ifdef SPROUT_PLATFORM_WIN

void main(int argc, char** argv)
{
	Sprout::Log::Init();

	Sprout::App* app = Sprout::CreateApp();
	app->Run();
	delete app;
}

#endif
