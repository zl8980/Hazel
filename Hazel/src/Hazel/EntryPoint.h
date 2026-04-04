#pragma once

#ifdef HZ_PLATFORM_WINDOWS

// 这是入口点文件，定义了程序的主函数
// extern 关键字声明了一个外部函数，该函数在其他编译单元中定义，这里只是告诉编译器它的存在
// 这样做的好处是，我们可以在这个文件中使用该函数，而不需要包含它的头文件
// EntryPoint是引擎的入口点，用户不需要关心它的实现细节
// 用户只需要实现 CreateApplication 函数，返回一个 Application 的指针
// 这样引擎就可以调用该函数，获取用户创建的 Application 对象，并运行它

// EntryPoint是引擎的入口点，用户需要实现 CreateApplication 函数
// 该函数返回一个 Application 的指针
// 这样引擎就可以调用该函数，获取用户创建的 Application 对象
// 并运行它，销毁它

// 这样做的好处是，用户只需要实现 CreateApplication 函数
// 引擎就可以调用该函数，获取用户创建的 Application 对象
// 并运行它，销毁它
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv)
{
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	int a = 5;
	HZ_INFO("Hello! Var={0}", a);

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif