#include "hzpch.h"
#include "Shader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hazel {
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
#pragma region VertexShader
		// 创建一个空的顶点着色器句柄  它会在GPU上分配显存 在显存上创建一个顶点着色器对象，
		// 并返回一个唯一的ID来标识这个对象。这个ID将用于后续的操作，如编译、链接和使用着色器。
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// 将顶点着色器源代码发送到GL
		// 注意，std::string的.c_str是以NULL字符结尾的。
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// 编译顶点着色器
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// 创建一个足够大的字符串来存储错误日志
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// 我们不再需要这个着色器了。
			glDeleteShader(vertexShader);


			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}
#pragma endregion

#pragma region FragmentShader
		// 创建一个空的片段着色器句柄
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// 将片段着色器源代码发送到GL
		// 注意，std::string的.c_str是以NULL字符结尾的。
		source = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// 编译片段着色器
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// 创建一个足够大的字符串来存储错误日志
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// 我们不再需要片段着色器了。
			glDeleteShader(fragmentShader);
			// 同样地，不要泄漏顶点着色器。
			glDeleteShader(vertexShader);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

#pragma endregion

#pragma region ShaderProgram
		// 顶点和片段着色器已成功编译。
		// 现在是时候将它们链接到一个程序中了。
		// 创建一个程序对象。
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;

		// 将我们的着色器附加到程序
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		// 链接我们的程序
		glLinkProgram(program);

		// 注意这里的不同函数：glGetProgramiv* 而不是 glGetShaderiv*。
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// 创建一个足够大的字符串来存储错误日志
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// 我们不再需要这个程序了。
			glDeleteProgram(program);
			// 同样地，不要泄漏着色器。
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);


			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		// 成功链接后，总是分离着色器。
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
#pragma endregion		
	}
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}


	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
	{
		//获取uniform变量在GPU中的位置
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		//将矩阵数据发送到GPU  第3个参数表示是否需要转置矩阵  
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}	