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
IncludeDir["glm"] = "BlueMarble/vendor/glm"
IncludeDir["stb_image"] = "BlueMarble/vendor/stb_image"

include "BlueMarble/vendor/GLFW"
include "BlueMarble/vendor/Glad"
include "BlueMarble/vendor/imgui"

project "BlueMarble"
	location "BlueMarble"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "bmpch.h"
	pchsource "BlueMarble/src/bmpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BM_BUILD_DLL",
			"BM_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "BM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BM_RELEASE" 
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BM_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"BlueMarble/src",
		"BlueMarble/vendor",
		"%{IncludeDir.glm}"
	}

	links
	{
		"BlueMarble"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"BM_PLATFORM_WINDOWS"
	 	}

	filter "configurations:Debug"
		defines "BM_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BM_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BM_DIST"
		runtime "Release"
		optimize "on"