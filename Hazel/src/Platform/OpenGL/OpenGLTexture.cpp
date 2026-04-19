#include "hzpch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"

namespace Hazel {


	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		HZ_PROFILE_FUNCTION();

		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;	
		
		//在GPU上创建一个2D纹理对象
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//在GPU上给 2D纹理对象 分配内存  MIPMAP层级数量为1 默认为0级纹理(原纹理)
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		//给GPU上2D纹理对象 指定参数  纹理过滤参数 放大 和 缩小时候 用什么算法
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}



	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		HZ_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		{
			HZ_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");

			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		HZ_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;


		//把数据上传到GPU
		//在GPU上创建一个2D纹理对象
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//在GPU上给 2D纹理对象 分配内存  MIPMAP层级数量为1 默认为0级纹理(原纹理)
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		//给GPU上2D纹理对象 指定参数  纹理过滤参数 放大 和 缩小时候 用什么算法
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//把CPU上的纹理数据 上传到 GPU上的 2D纹理对象的显存中
		//参数1：纹理对象ID
		//参数2：mipmap级别，0表示基本图像级别(原始纹理)
		//参数3：纹理子区域的x偏移量，CPU上的纹理数据从纹理的左边界x位置开始上传
		//参数4：纹理子区域的y偏移量，CPU上的纹理数据从纹理的上边界y位置开始上传
		//参数5：纹理子区域的宽度
		//参数6：纹理子区域的高度
		//参数7：像素数据的格式，GL_RGB表示每个像素包含红、绿、蓝三个分量
		//参数8：像素数据的类型，CPU上的数据 GL_UNSIGNED_BYTE表示每个分量使用无符号字节存储 
		//参数9：指向像素数据的指针
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		//CPU上的纹理数据已经上传到GPU上了，CPU上的纹理数据就可以释放了
		stbi_image_free(data);

	}


	OpenGLTexture2D::~OpenGLTexture2D()
	{
		HZ_PROFILE_FUNCTION();

		//在GPU上删除纹理对象，释放GPU上的纹理资源
		glDeleteTextures(1, &m_RendererID);
	}	

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		HZ_PROFILE_FUNCTION();

		//把GPU上的纹理对象绑定到当前的OpenGL上下文中，使其成为当前活动的纹理对象		
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		HZ_PROFILE_FUNCTION();

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

}