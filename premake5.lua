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

group "Dependencies"
	include "Sprout/vendor/glfw"
	include "Sprout/vendor/glad"
	include "Sprout/vendor/imgui"
group ""

include "Sprout"
include "Sandbox"