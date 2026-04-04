#include "hzpch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"
namespace Hazel
{


	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f)),m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		//平移矩阵  * 旋转矩阵(按Z轴(0,0,1)旋转的角度)  ==》 变换矩阵
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1)); 

		//通过矩阵的逆变换  ==》 视图矩阵
		m_ViewMatrix = glm::inverse(transform);

		//视图投影矩阵
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}