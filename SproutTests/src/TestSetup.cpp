#include "CppUnitTest.h"
#include "Sprout/Core/Log.h"

namespace SproutTests
{
    TEST_MODULE_INITIALIZE(InitializeTests)
    {
        Sprout::Log::Init();
    }
}