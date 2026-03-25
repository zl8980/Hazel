workspace "Hazel" --相当于vs的解决方案
	architecture "x64"		--架构	
	startproject "Sandbox"	--设置启动项目
	configurations			--配置
	{
		"Debug",
		"Release",			--也是一种调试模式	但是去除了大量的调试信息 启用了优化
		"Dist"				--分发版本	完全去除了所有的内容
	}

--创建变量	通过premake中提供的标记来设置	里面有很多宏)
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--通过表结构来封装	库名	和	路径
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"
IncludeDir["glm"] = "Hazel/vendor/glm"


--我们已经有的预编译文件 在这个目录中存在premake5.lua	这种包含 会复制粘贴到我们的文件内容中
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"




--创建Hazel项目
project "Hazel"
	location "Hazel"	--在premake5.lua 文件所在的文件夹中来定位该目录
	kind "StaticLib"	--编译生成什么类型的文件	在premake中	动态库	和	共享库是相同的
	language "C++"		--语言是C++
	cppdialect "C++17"	--C++版本		
	staticruntime "on"	--静态链接库	需要使用静态链接的运行时库	也就是LIB文件 


	targetdir("bin/" .. outputdir .. "/%{prj.name}")			--应用输出目录
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")		--中间文件输出目录

	--预编译头文件
	pchheader "hzpch.h" 
	pchsource "Hazel/src/hzpch.cpp" --在其他的平台被忽略 只能在Visual Studio中使用	其他平台会当做普通的cpp文件来编译


	--被编译的源文件 **是递归文件夹下所有的文件
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	--第三方的头文件路径 以及 本项目的相关路径
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	--链接库
	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}


	--过滤器  只是在windows系统下
	filter "system:windows"		
		systemversion "latest"  --用最新版本
		
		--定义编译宏
		defines
		{
			"HZ_PLATFORM_WINDOWS",
			"HZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE", --当#include glfw 头文件的时候  不会包含任何OpenGL的头文件
		}
		

	--过滤器 用于调试          所有的系统平台
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"		--运行时库  调试版本
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"	--运行时库  发布版本
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"	--运行时库  发布版本
		optimize "on"





--创建Sandbox项目
project "Sandbox"
	location "Sandbox"	--在premake5.lua 文件所在的文件夹中来定位  
	kind "ConsoleApp"	--编译生成什么类型的文件
	language "C++"		--语言是C++
	cppdialect "C++17"	--C++版本		
	staticruntime "on"	--动态链接库	需要使用动态链接的运行时库	也就是DLL文件
	targetdir("bin/" .. outputdir .. "/%{prj.name}")			--应用输出目录
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")		--中间文件输出目录

	--被编译的源文件 **是递归文件夹下所有的文件
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	--头文件
	includedirs
	{
		"Hazel/vendor/spdlog/include",
		"Hazel/src",
		"Hazel/vendor",
		"%{IncludeDir.glm}"
	}

	--链接库文件
	links
	{
		"Hazel"
	}

	--过滤器  只是在windows系统下
	filter "system:windows"		
		systemversion "latest"	--用最新版本

		--定义编译宏
		defines
		{
			"HZ_PLATFORM_WINDOWS"
		}		

	--过滤器 用于调试		所有的系统平台
	filter "configurations:Debug"
		defines "HZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "HZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZ_DIST"
		runtime "Release"
		optimize "on"

		