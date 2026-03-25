#pragma once

#include "Hazel/Window.h"

#include <GLFW/glfw3.h>  //不会包含任何OpenGL的头文件  因为在premake5.lua中定义了"GLFW_INCLUDE_NONE"

namespace Hazel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		//更新 我们更新GLFW的交换缓冲区 每帧调用一次
		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//设置事件回调
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		//设置垂直同步
		void SetVSync(bool enabled) override;
		//是否垂直同步
		bool IsVSync() const override;
		//获取原生窗口
		inline void* GetNativeWindow() const override { return m_Window; }
	private:
		//初始化  
		virtual void Init(const WindowProps& props);
		//关闭
		virtual void Shutdown();
	private:
		//定义一个GLFW窗口
		GLFWwindow* m_Window;

		//窗口数据  --把这个传给GLFW
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}