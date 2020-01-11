workspace "RTR_Solution"

	-- System architecture (either 32 or 64 bit)
	architecture "x86"
	startproject "RTR_Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

-- Output directories
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "external/GLFW/include"
IncludeDir["GLEW"] = "external/GLEW/include"
IncludeDir["ASSIMP"] = "external/ASSIMP/include"
IncludeDir["GLM"] = "external/GLM/glm"
IncludeDir["stb_image"] = "external/STB/include"


project "RTR_Engine"
	location "RTR_Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	-- Adds files to a project
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	-- Specifies the include file search paths for the compiler
	includedirs
	{
		"external/SPDLOG/include";	
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.ASSIMP}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.stb_image}"
	}

	-- Sets additional dependencies (.lib files)
	links 
	{ 
		"opengl32",	
		"glew32",
		"glfw3",
		"assimp-vc141-mtd"
	}

	 -- For 32 bit use these library paths
    libdirs { 
		"external/ASSIMP/lib",
		"external/GLEW/lib/win32",
		"external/GLFW/lib-vc2017"
	 }

	-- Build settings for windows
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

	-- Build configurations
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

	-- Specifies the include file search paths for the compiler
	includedirs
	{
		"external/SPDLOG/include";
		"RTR_Engine/src"
	}

	links 
	{
		"RTR_Engine"
	}

	-- Build settings for windows
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RTR_PLATFORM_WINDOWS"
		}

	-- Build configurations
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