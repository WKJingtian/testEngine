workspace "Tengine"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	startproject "Sandbox"

output_dir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "dependencies/glad"
include "dependencies/imgui"
include "dependencies/yaml"

project "Tengine"
	location "Tengine"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
	cppdialect "C++17"
	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	pchheader "tpch.h"
	pchsource "Tengine/src/tpch.cpp"

	files
	{
		"dependencies/**.lib",
		"%{prj.name}/**.cpp",
		"%{prj.name}/**.h"
	}

	includedirs
	{
		"dependencies/glad/",
		"dependencies/glfw/",
		"dependencies/imgui/",
		"dependencies/glm/",
		"dependencies/entt/",
		"dependencies/yaml/",
		"dependencies/yaml/include",
		"dependencies/",
		"%{prj.name}/include/",
		"%{prj.name}/"
	}
	links
	{
		"yaml",
		"imgui",
		"glad",
		"OpenGL32",
		"glfw3dll"
	}
	libdirs
	{
		"dependencies/yaml/",
		"dependencies/imgui/",
		"dependencies/glad/",
		"dependencies/glfw/"
	}
	postbuildcommands
	{
		("{COPY} ../dependencies/glfw/glfw3.dll ../bin/" .. output_dir .. "/%{prj.name}")
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS";
			"BUILD_MY_DLL";
			"GLFW_INCLUDE_NONE";
			"STB_IMAGE_IMPLEMENTATION";
		}

	filter "configurations:Debug"
		defines "T_DEBUG"
		buildoptions "/MDd"
--		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "T_RELEASE"
		buildoptions "/MD"
--		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "T_DIST"
		buildoptions "/MD"
--		runtime "Release"
		optimize "on"

project "Editor"

	location "Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.cpp",
		"%{prj.name}/**.h"
	}

	includedirs
	{
		"dependencies/glad/",
		"dependencies/glfw/",
		"dependencies/imgui/",
		"dependencies/glm/",
		"dependencies/entt/",
		"dependencies/",
		"%{prj.name}/include/",
		"%{prj.name}/",
		"Tengine/include/",
		"Tengine/"
	}

	links
	{
		"Tengine",
	}
	postbuildcommands
	{
		("{COPY} ../dependencies/glfw/glfw3.dll ../bin/" .. output_dir .. "/%{prj.name}"),
		("{COPY} ../Editor/asset ../bin/" .. output_dir .. "/%{prj.name}/asset/")
	}


	filter "system:windows"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS";
		}

	filter "configurations:Debug"
		defines "T_DEBUG"
--		runtime "Debug"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "T_RELEASE"
--		runtime "Release"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "T_DIST"
--		runtime "Release"
		buildoptions "/MD"
		optimize "on"