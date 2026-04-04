#pragma once
#include "glm/glm.hpp"

namespace Hazel
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		//设置摄像机的位置
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		//获取摄像机的位置
		const glm::vec3& GetPosition() const { return m_Position; }

		//设置摄像机 绕Z轴旋转的角度 
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		//获取摄像机 绕Z轴的旋转角度
		float GetRotation() { return m_Rotation; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		//重新计算视图矩阵
		void RecalculateViewMatrix();


	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;


		glm::vec3 m_Position = glm::vec3(0.0f);
		float m_Rotation = 0.0f;			//固定绕Z轴旋转
	};

}


