#pragma once

#include "Core.h"
#define FMT_UNICODE 0
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Hazel {
	
	class HAZEL_API Log
	{
	public:
		static void Init();
		//这里的内联有什么作用呢？  其实就是在编译阶段 就把函数展开  这样就不需要函数调用的开销了
		//内联可以让我们在.h文件中定义函数  这样就可以在任何需要使用日志的地方 包含这个头文件 就可以直接使用日志了 
		//否则呢 我们需要在.cpp文件中定义函数  然后在.h文件中声明函数  这样就需要函数调用的开销了
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}
//这些宏在什么时候展开呢？  在编译阶段  当编译器遇到这些宏的时候 就会把它们展开成对应的函数调用  
//这样就可以让我们在代码中使用这些宏来进行日志输出了
// Core log macros
#define HZ_CORE_TRACE(...)    ::Hazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)     ::Hazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)     ::Hazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)    ::Hazel::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_FATAL(...)    ::Hazel::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define HZ_TRACE(...)	      ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)	      ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)	      ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)	      ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZ_FATAL(...)	      ::Hazel::Log::GetClientLogger()->fatal(__VA_ARGS__)


//把他们设置成宏的好处 就是在发布版本的时候  我们可以通过定义一个宏来禁用日志输出  这样就可以减少日志输出的开销了