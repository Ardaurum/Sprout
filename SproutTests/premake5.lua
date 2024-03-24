project "SproutTests"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"
	externalanglebrackets "On"
	externalwarnings "Off"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{wks.location}/Sprout/src",
		"%{wks.location}/Sprout/vendor",
		"%{wks.location}/Sprout/vendor/spdlog/include",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Sprout"
	}

	copysrcDbg = path.join("%{wks.location}", "%{sdldir}", "Debug", "SDL3.dll")
	copysrcRel = path.join("%{wks.location}", "%{sdldir}", "Release", "SDL3.dll")

	filter "configurations:Debug"
		defines { "SPROUT_DEBUG" }
		runtime "Debug"
		symbols "On"
		postbuildcommands
		{
			"{COPYFILE} %{copysrcDbg} %{cfg.targetdir}"
		}

	filter "configurations:Release"
		defines { "SPROUT_RELEASE" }
		runtime "Release"
		optimize "On"
		postbuildcommands
		{
			"{COPYFILE} %{copysrcRel} %{cfg.targetdir}"
		}

	filter "system:windows"
		systemversion "latest"
