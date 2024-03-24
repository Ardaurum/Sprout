#pragma once

#include "Log.h"
#include "PlatformDetection.h"
#include "App.h"
#include "CLISystem.h"

#include <iostream>

#ifdef SPROUT_DEBUG_MEMORY_LEAKS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <iostream>
#endif

extern Sprout::App* Sprout::CreateApp();
extern void Sprout::DefineGameCLIArgs();

#ifdef SPROUT_PLATFORM_WIN

void main(int argc, char** argv)
{
	Sprout::Log::Init();

	Sprout::DefineCLIArgs();
	Sprout::CLISystem::Get().ParseArgs(argc, argv);

#ifdef SPROUT_DEBUG_MEMORY_LEAKS
	_CrtMemState memStart;
	_CrtMemState memEnd;
	_CrtMemState memDiff;

	_CrtMemCheckpoint(&memStart);
#endif

	Sprout::App* app = Sprout::CreateApp();

	app->Run();
	delete app;

#ifdef SPROUT_DEBUG_MEMORY_LEAKS
	_CrtMemCheckpoint(&memEnd);
	if (_CrtMemDifference(&memDiff, &memStart, &memEnd))
	{
		SPROUT_CORE_WARN("Detected Memory Leak! Check VS console for more info!");
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); 
		_CrtMemDumpStatistics(&memDiff);
	}
#endif
}

#endif
