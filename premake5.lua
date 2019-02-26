workspace "BlueMarble"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder
IncludeDir = {}
IncludeDir["GLFW"] = "BlueMarble/vendor/GLFW/include"
IncludeDir["Glad"] = "BlueMarble/vendor/Glad/include"
IncludeDir["ImGui"] = "BlueMarble/vendor/imgui"

include "BlueMarble/vendor/GLFW"
include "BlueMarble/vendor/Glad"
include "BlueMarble/vendor/imgui"

project "BlueMarble"
	location "BlueMarble"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bmpch.h"
	pchsource "BlueMarble/src/bmpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BM_BUILD_DLL",
			"BM_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "BM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "BM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "BM_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"BlueMarble/vendor/spdlog/include",
		"BlueMarble/src"
	}

	links
	{
		"BlueMarble"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BM_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BM_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "BM_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "BM_DIST"
		runtime "Release"
		optimize "On"