workspace "DX11Renderer"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["stb"] = "vendor/stb"
IncludeDir["imgui"] = "vendor/imgui"

project "DX11Renderer"
	location "DX11Renderer"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"vendor/imgui/imconfig.h",
		"vendor/imgui/imgui.h",
		"vendor/imgui/imgui.cpp",
		"vendor/imgui/imgui_draw.cpp",
		"vendor/imgui/imgui_internal.h",
		"vendor/imgui/imgui_tables.cpp",
		"vendor/imgui/imgui_widgets.cpp",
		"vendor/imgui/imstb_rectpack.h",
		"vendor/imgui/imstb_textedit.h",
		"vendor/imgui/imstb_truetype.h",
		"vendor/imgui/imgui_demo.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		IncludeDir["stb"],
		IncludeDir["imgui"]
	}


	links
	{
		"d3d11.lib",
		"d3dcompiler.lib",
		"dxgi.lib"
	}

	shadermodel ("5.0")
	shaderobjectfileoutput ("%{prj.name}/src/Shaders/CSO/%%(Filename).cso")

	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"