call cmake -S Sprout\vendor\sdl3 -B Sprout\vendor\sdl3-build -G "Visual Studio 17 2022"
call vendor\premake\premake5.exe vs2022
PAUSE