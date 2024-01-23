#pragma once

#include "Log.h"
#include "PlatformDetection.h"
#include "App.h"
#include "CLISystem.h"

#include <iostream>

extern Sprout::App* Sprout::CreateApp();
extern void Sprout::DefineGameCLIArgs();

#ifdef SPROUT_PLATFORM_WIN

void main(int argc, char** argv)
{
	Sprout::Log::Init();

	Sprout::DefineCLIArgs();
	Sprout::CLISystem::Get().ParseArgs(argc, argv);

	Sprout::App* app = Sprout::CreateApp();
	app->Run();
	delete app;
}

#endif
