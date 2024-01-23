include "Dependencies.lua"

workspace "Sprout"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

sdldir = "Sprout/vendor/sdl3-build"

group "Dependencies"
	include "Sprout/vendor/imgui"
	-- SDL3
	externalproject "SDL3-shared"
		location "%{sdldir}"
		uuid "90FB24FD-91F6-3207-9258-FB86DA45D127"
		kind "SharedLib"
		language "C++"
		includedirs { "Sprout/vendor/sdl3/include"}
group ""

include "Sprout"
include "Sandbox"