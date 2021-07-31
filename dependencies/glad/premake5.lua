project "glad"
	kind "staticLib"
	language "C"
	staticruntime "on"
	targetdir ("bin/" .. output_dir .. "/%{prj.name}")
	objdir ("bin-int/" .. output_dir .. "/%{prj.name}")
	files
	{
		"glad.c",
		"glad.h",
		"khrplatform.h"
	}
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		buildoptions "/MDd"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		buildoptions "/MD"
		runtime "Debug"
		symbols "on"