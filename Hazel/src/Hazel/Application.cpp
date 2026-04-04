#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"

#include "Hazel/Renderer/Renderer.h"

#include "Input.h"

#include "glfw/glfw3.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		//创建窗口 m_Window
		m_Window = std::unique_ptr<Window>(Window::Create());
		//让 Application 监听窗口产生的所有事件 ==>窗口事件发生时通知我们
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);		
	}


	Application::~Application()
	{
	}

	//推入普通层
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	//推入覆盖层
	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}




	//处理所有传入的事件
	void Application::OnEvent(Event& e)
	{
		//创建事件分发器 传入事件e 准备分发
		EventDispatcher dispatcher(e);
		//如果事件e的类型匹配到 WindowCloseEvent 类型  
		//OnWindowClose就会被调用来处理这个关闭事件
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		//打印事件e的详细信息
		//HZ_CORE_TRACE("{0}", e.ToString());
		//从顶层往底层遍历 逆序遍历层栈 -- 覆盖层优先 普通层靠后 最先响应事件
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);//先自减再解引用 因为end() = null  指向最后一个元素的下一个位置
			if (e.Handled)//如果事件已经被某一层处理 就不再向下传递事件  停止继续传播
				break;
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime(); //用glfw 在该平台上获取当前时间 以秒为单位 
			Timestep timestep = time - m_LastFrameTime; 
			m_LastFrameTime = time;

			//使用范围based for循环    遍历整个m_LayerStack层栈 从 begin() 到 end()  
			//正序遍历 -- 底层到顶层   从第0个层 -- 第n-1个层
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	//处理窗口关闭事件
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}