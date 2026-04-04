#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

	LayerStack::LayerStack()
	{

	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	// 推入普通层，插入到 m_LayerInsertIndex 所指向的位置之前
	void LayerStack::PushLayer(Layer* layer)
	{
		// 在插入位置处，将新层插入到普通层区域的末尾，覆盖层区域的前面
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	// ooooooxxx  o表示普通层  x表示覆盖层  m_LayerInsert 指向o层最后一个位置的下一个位置
	// 弹出时	找到对应的普通层o  将该o层从列表中移除1个
	// oooooxxx m_LayerInsert-- 指向o层最后一个位置的下一个位置
	void LayerStack::PopLayer(Layer* layer)
	{
		// 在普通层区域内查找
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);         // 移除该层
			m_LayerInsertIndex--;       // 更新普通层的插入位置
		}
	}

	// ooooooxxx  o表示普通层  x表示覆盖层  覆盖层直接从末尾查找删除
	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}

}