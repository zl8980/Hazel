#include "hzpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h> 
#include "glad/glad.h"
#include "GL/GL.h"
namespace Hazel {
	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	
	void OpenGLContext::Init()
	{
		//设置GLFW窗口的上下文 == OpenGl上下文 并与 窗口进行绑定
		glfwMakeContextCurrent(m_WindowHandle);
		//加载OpenGL函数指针  这里的gladLoadGLLoader函数  用来加载OpenGL函数指针
		//					  这里的glfwGetProcAddress函数  用来获取OpenGL函数地址
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initailize Glad");	

		HZ_CORE_INFO("Vendor:{0}", (char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("Renderer:{0}", (char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("Version:{0}", (char*)glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwPollEvents();//拉取事件  处理事件
		//交换缓冲区  这里的SwapBuffers函数  是我们在OpenGLContext类中实现的  通过GLFW来交换缓冲区
		glfwSwapBuffers(m_WindowHandle);//交换缓冲区
	}
}