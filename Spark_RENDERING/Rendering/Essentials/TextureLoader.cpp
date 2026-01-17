#include "TextureLoader.h"
#include <SOIL/SOIL.h>
#include <Logger/Logger.h>

namespace SPARK_RENDERING
{
    bool TextureLoader::LoadTexture(const std::string& filepath, GLuint& id, int& width, int& height, bool blended)
    {
		int channels = 0;

		unsigned char* image = SOIL_load_image(
			filepath.c_str(),					// 文件名
			&width,								// 图像宽度
			&height,							// 图像高度
			&channels,							// 图像通道数
			SOIL_LOAD_AUTO						// 加载模式
		);

		// Check if the image was loaded successfully
		if (!image)
		{
			std::string error = SOIL_last_result();
			SPARK_ERROR("SOIL failed to load image [{0}] -- {1}", filepath, error);
			return false;
		}

		GLint format = GL_RGBA;

		switch (channels)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			format = GL_RGBA;
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if (!blended)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glTexImage2D(
			GL_TEXTURE_2D,			// 目标纹理类型
			0,						// 细节级别
			format,					// 纹理内部格式
			width,					// 纹理宽度
			height,					// 纹理高度
			0,						// 边框宽度
			format,					// 源图像格式
			GL_UNSIGNED_BYTE,		// 源图像数据类型
			image					// 图像数据
		);

		// Delete the image data after generating the texture
		SOIL_free_image_data(image);

		return true;
    }

    std::shared_ptr<Texture> TextureLoader::Create(Texture::TextureType type, const std::string& texturePath)
    {
		GLuint id;
		int width, height;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		switch (type)
		{
		case Texture::TextureType::PIXEL:
			LoadTexture(texturePath, id, width, height, false);
			break;
		case Texture::TextureType::BLENDED:
			LoadTexture(texturePath, id, width, height, true);
			break;
		// TODO: 添加其他纹理类型的支持
		default:
			assert(false && "The current type is not defined, Please use a defined texture type!");
			return nullptr;
		}
        return std::make_shared<Texture>(id, width, height, type, texturePath);
    }
}