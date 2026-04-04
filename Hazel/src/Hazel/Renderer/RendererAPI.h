#pragma once
#include "glm/glm.hpp"
#include "VertexArray.h"

namespace Hazel
{
	class RendererAPI
	{
	public://类型相关
		enum class API
		{
			None = 0,
			OpenGL = 1,
			Direct3D = 2,
			Metal = 3,
			Vulkan = 4
		};
	public://方法相关
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private://私有成员
		static API s_API;
	};
	

}