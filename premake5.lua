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
IncludeDir["assimp"] = "vendor/assimp/include"

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
		"%{prj.name}/src/Shaders/HLSL/**.hlsl",
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
		"vendor/imgui/imgui_demo.cpp",
		"vendor/assimp/**.h",
		"vendor/assimp/**.hpp",
		"vendor/assimp/**.cpp",
		"vendor/assimp/**.c",
		"vendor/assimp/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		IncludeDir["stb"],
		IncludeDir["imgui"],
		IncludeDir["assimp"]
	}


	links
	{
		"d3d11.lib",
		"d3dcompiler.lib",
		"dxgi.lib",
		"dxguid.lib",
		"%{wks.location}/vendor/assimp/lib/assimp-vc143-mt.lib"
	}

	postbuildcommands
	{
		"{COPYDIR} \"%(ProjectDir)src/Assets\" %[%{!cfg.targetdir}/src/Assets]",
		"{COPYDIR} \"%(ProjectDir)src/Shaders/CSO\" %[%{!cfg.targetdir}/src/Shaders/CSO]",
		"{COPYDIR} %[%{!wks.location}vendor/assimp/bin] %[%{!cfg.targetdir}/]"
	}

	-- Custom HLSL Compilation Step
	filter "files:**Shaders/HLSL/Deferred/**VS.hlsl"
	buildcommands
	{
		"echo Compiling Compute Shader: %(FullPath)",
		"fxc /T vs_5_0 /Fo \"%(ProjectDir)src/Shaders/CSO/Deferred/%(Filename).cso\" /nologo \"%(FullPath)\""
	}
	buildoutputs 
	{
		"%{prj.name}/src/Shaders/CSO/Deferred/%(Filename).cso"
	}
	filter "files:**Shaders/HLSL/Deferred/**PS.hlsl"
	buildcommands
	{
		"echo Compiling Compute Shader: %(FullPath)",
		"fxc /T ps_5_0 /Fo \"%(ProjectDir)src/Shaders/CSO/Deferred/%(Filename).cso\" /nologo \"%(FullPath)\""
	}
	buildoutputs 
	{
		"%{prj.name}/src/Shaders/CSO/Deferred/%(Filename).cso"
	}
	filter "files:**Shaders/HLSL/**VS.hlsl"
		buildcommands
		{
			"echo Compiling Vertex Shader: %(FullPath)",
			"fxc /T vs_5_0 /Fo \"%(ProjectDir)src/Shaders/CSO/%(Filename).cso\" /nologo \"%(FullPath)\""
		}
		buildoutputs 
		{
			"%{prj.name}/src/Shaders/CSO/%(Filename).cso"
		}
		
	filter "files:**Shaders/HLSL/**PS.hlsl"
		buildcommands
		{
			"echo Compiling Pixel Shader: %(FullPath)",
			"fxc /T ps_5_0 /Fo \"%(ProjectDir)src/Shaders/CSO/%(Filename).cso\" /nologo \"%(FullPath)\""
		}
		buildoutputs 
		{
			"%{prj.name}/src/Shaders/CSO/%(Filename).cso"
		}
	filter "files:**Shaders/HLSL/Compute/**CS.hlsl"
		buildcommands
		{
			"echo Compiling Compute Shader: %(FullPath)",
			"fxc /T cs_5_0 /Fo \"%(ProjectDir)src/Shaders/CSO/Compute/%(Filename).cso\" /nologo \"%(FullPath)\""
		}
		buildoutputs 
		{
			"%{prj.name}/src/Shaders/CSO/Compute/%(Filename).cso"
		}

	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"