#pragma once

#include "hzpch.h"
#include "Hazel/Core.h"


namespace Hazel {

	/*
	Hazel 中的事件目前是阻塞式的，这意味着当事件发生时，它会立即被分发，并且必须在当时立即处理。
	从长远来看，更好的策略可能是将事件缓冲到事件总线中，并在更新阶段的“事件”部分进行处理。
	*/

	//事件类型
	enum class EventType  
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, //窗口相关事件
		AppTick, AppUpdate, AppRender,                                        //App相关的事件
		KeyPressed, KeyReleased,KeyTyped,                                     //键盘相关的事件
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled    //鼠标相关的事件
	};

	//事件类别  用于过滤某类事件  可以将单个事件 应用多个类别
	//鼠标按钮事件 也是鼠标事件  键盘事件 也属于输入事件
	enum EventCategory 
	{
		None = 0,
		EventCategoryApplication =  BIT(0),
		EventCategoryInput =        BIT(1),
		EventCategoryKeyboard =     BIT(2),
		EventCategoryMouse =        BIT(3),
		EventCategoryMouseButton =  BIT(4)
	};

//由于输入这些事件名称 类型  ---很麻烦 而且大部分代码相似  所以用宏来对它进行扩展
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
//通过宏来 实现  类别虚函数
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }



	//事件的基类
	class HAZEL_API Event
	{		
	public:
		bool Handled = false;									  //查看事件是否被处理
		virtual EventType GetEventType() const = 0;               //获取事件类型
		virtual const char* GetName() const = 0;                  //获取事件名称  一般用在调试
		virtual int GetCategoryFlags() const = 0;                 //获取类别标志
		virtual std::string ToString() const { return GetName(); }//默认是返回事件名称  你可以重写这个函数

		//该事件是否属于  某种类别的事件
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	};

	//事件分发器
	class EventDispatcher
	{
		template<typename T>//T可以是任何的事件类型 
		using EventFn = std::function<bool(T&)>; //这是一个标准的函数  返回值:Bool类型  输入参数: 是模板参数的引用T&
	public:
		EventDispatcher(Event& event)//当事件分发器被构建时候 它接受一个参数 是事件的引用  --它可以是任意事件
			: m_Event(event)
		{
		}

		//构造完分发器  我们可以多次调用这个函数
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	//可以轻松的调用事件的 ToString方法  
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}

