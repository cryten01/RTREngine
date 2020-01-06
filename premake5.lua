workspace "RTR_Solution"
	architecture "x86"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "RTR_Engine"
	location "RTR_Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"external/SPDLOG/include";	
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RTR_PLATFORM_WINDOWS",
			"RTR_BUILD_DLL"
		}

		postbuildcommands
		{
			-- copies RTR_Engine.dll into RTR_Sandbox executable folder
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/RTR_Sandbox")
		}

	filter "configurations:Debug"
		defines "RTR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "RTR_DISTRIBUTION"
		runtime "Release"
		optimize "on"


project "RTR_Sandbox"
	location "RTR_Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

		files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"external/SPDLOG/include";
		"RTR_Engine/src"
	}

	links 
	{
		"RTR_Engine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RTR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "RTR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "RTR_DISTRIBUTION"
		runtime "Release"
		optimize "on"