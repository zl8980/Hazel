#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include "glad/glad.h"

namespace Hazel {
#pragma region //全局变量和静态函数
	static bool s_GLFWInitialized = false;
	//打印错误码 和 错误信息
	static void GLFWErrorCallback(int error, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}


	//与平台无关  基类接口实现
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
#pragma endregion

	



	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		//这里的判断是为了  初始化GLFW一次
		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();//初始化GLFW
			//如果开启断言 失败的时候会有断点 
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");
			//当GLFW出现错误  会回调该 GLFWErrorCallback
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		//通过GLFW_API 创建 GLFW窗口对象 与之关联的 OpenGL 上下文。
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		//设置GLFW窗口的上下文 == OpenGl上下文 并与 窗口进行绑定
		glfwMakeContextCurrent(m_Window);
		//加载OpenGL函数指针  这里的gladLoadGLLoader函数  用来加载OpenGL函数指针  
		//					  这里的glfwGetProcAddress函数  用来获取OpenGL函数地址		
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initailize Glad");
		//给GLFW窗口 设置 自定义窗口数据
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);



#pragma region //设置GLFW各种 回调函数
		//给GLFW窗口 设置 窗口大小改变时候的 回调函数
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				//从GLFW窗口 获取 自定义窗口数据
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				//创建窗口尺寸大小改变的事件
				WindowResizeEvent event(width, height);
				//通过应用层 处理事件函数  来做处理
				data.EventCallback(event);
			});
		//给GLFW窗口 设置 窗口的关闭时候的 回调函数
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				//创建窗口关闭的事件
				WindowCloseEvent event;
				data.EventCallback(event);
			});
		//给GLFW窗口 设置 键盘事件 回调函数
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:		//键盘按键按下
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:		//键盘按键抬起
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:		//键盘按键按下直到重复
				{
					KeyPressedEvent event(key, 1);//这里先设置为1  后续我们可以根据需要  来设置不同的值  来表示不同的重复次数
					data.EventCallback(event);
					break;
				}
				}
			});

		//给GLFW窗口 设置 键盘输入事件 回调函数
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		//给GLFW窗口 设置 鼠标事件 回调函数
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:			//鼠标按键按下
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:			//鼠标按键抬起
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});
		//给GLFW窗口 设置 鼠标滚轮滚动事件 回调函数
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});
		//给GLFW窗口 设置 鼠标移动事件 回调函数
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
#pragma endregion
		

	}

	void WindowsWindow::Shutdown()
	{
		//通过GLFW来销毁窗口
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();//拉取事件  处理事件
		glfwSwapBuffers(m_Window);//交换缓冲区
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)//垂直同步  交换间隔设置为1 会等待1帧的时间  交换间隔设置为0 就不等待  直接交换
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	//是否启用了垂直同步
	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
