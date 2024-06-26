project "Sprout"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"
	externalanglebrackets "On"
	externalwarnings "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "spch.h"
	pchsource "src/spch.cpp"

	defines
	{
		"SPDLOG_WCHAR_TO_UTF8_SUPPORT"
	}

	files 
	{
		"src/**.cpp",
		"src/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/stb_image/**.h"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.sdl}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"SDL3-shared",
		"imgui",
		"d3d11",
		"d3d12",
		"dxgi",
		"d3dcompiler"
	}

	filter "configurations:Debug"
		defines "SPROUT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SPROUT_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "SPROUT_DIST"
		runtime "Release"
		optimize "On"

	filter "system:windows"
		systemversion "latest"

		defines 
		{
			"SPROUT_DX11"
		}
