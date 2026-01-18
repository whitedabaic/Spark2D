#pragma once
#include <map>
#include <memory>
#include <string>

#include <Rendering/Essentials//Shader.h>
#include <Rendering/Essentials/Texture.h>

namespace SPARK_RESOURCES {
	class AssetManager
	{
	private:
		std::map<std::string, std::shared_ptr<SPARK_RENDERING::Texture>> m_mapTextures{};
		std::map<std::string, std::shared_ptr<SPARK_RENDERING::Shader>> m_mapShaders{};
	public:
		AssetManager() = default;
		~AssetManager() = default;

		bool AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt = true);
		const SPARK_RENDERING::Texture& GetTexture(const std::string& textureName);

		bool AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		SPARK_RENDERING::Shader& GetShader(const std::string& shaderName);
	};
}