#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	//图层 - 用于 渲染(UI层 场景层 关卡层 编辑 等等) 和 事件层  事件接收顺序
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer"); //
		virtual ~Layer();

		virtual void OnAttach() {}				//图层被压入堆栈  《初始化》
		virtual void OnDetach() {}				//图层被弹出堆栈   《关闭》
		virtual void OnUpdate() {}				//当图层需要更新的时候  应用程序会调用OnUpdate
		virtual void OnImGuiRender() {}			//当需要渲染ImGui的时候 会调用该函数
		virtual void OnEvent(Event& event) {}	//当有事件 发送过来的时候 会调用该函数
		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;			//在调试阶段有 层的名称
	};

}