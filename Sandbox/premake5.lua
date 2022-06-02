project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"
	
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

	filter "configurations:Debug"
		defines { "SPROUT_DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "SPROUT_RELEASE" }
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines { "SPROUT_DIST" }
		runtime "Release"
		optimize "On"

	filter "system:windows"
		systemversion "latest"