#pragma once

#include "hzpch.h"

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"


//平台无关的窗口
namespace Hazel {

	struct WindowProps
	{
		std::string Title;				//窗口标题
		unsigned int Width;				//窗口宽
		unsigned int Height;			//窗口高

		WindowProps(const std::string& title = "Hazel Engine",
			        unsigned int width = 1280,
			        unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	//基于窗口的界面系统 接口类
	class HAZEL_API Window
	{
	public:
		//事件回调函数
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// 窗口属性
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void* GetNativeWindow() const = 0;

		//这个创建窗口的函数  与平台无关 每个平台都会有自己的实现
		static Window* Create(const WindowProps& props = WindowProps());
	};

}