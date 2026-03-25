--创建Glad项目
project "Glad"
    kind "StaticLib"    --编译生成什么类型的文件 生成静态库文件
    language "C"        --语言是C
    staticruntime "on"  --静态链接库 需要使用静态链接的运行时库 也就是LIB文件
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")   --应用输出目录
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")  --中间文件输出目录


    --被编译的源文件
    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }
    --头文件
    includedirs
    {
        "include"
    }
    --过滤器  只是在windows系统下
    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

        filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"