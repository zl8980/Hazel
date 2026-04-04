#include "hzpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel {
	// 静态成员变量需要在类外进行初始化  在.cpp文件中进行定义  类中只是声明  这是C++的规定  静态成员变量是属于类的
	// 而不是属于对象的  所以需要在类外进行定义  这样就可以在任何地方使用这个静态成员变量了
	// 静态成员变量的定义  需要加上类名和作用域符号  s_CoreLogger是静态成员变量  需要在类外定义  不能在类内定义
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}