#include "AssetManager.h"
#include <Rendering/Essentials/TextureLoader.h>
#include <Rendering/Essentials/ShaderLoader.h>
#include <Logger/Logger.h>

namespace SPARK_RESOURCES {
    bool AssetManager::AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt)
    {
		//Check if texture already exists
        if (m_mapShaders.find(textureName) != m_mapShaders.end())
        {
            SPARK_WARN("Texture with name '{0}' already exists!", textureName);
            return false;
		}
        
        auto texture = SPARK_RENDERING::TextureLoader::Create(
			pixelArt ? SPARK_RENDERING::Texture::TextureType::PIXEL : SPARK_RENDERING::Texture::TextureType::BLENDED,
            texturePath
        );

        if (!texture)
        {
            SPARK_ERROR("Failed to load texture {0} at path [{1}]", textureName, texturePath);
            return false;
		}

		m_mapTextures.emplace(textureName, std::move(texture));
        return true;
    }

    std::shared_ptr<SPARK_RENDERING::Texture> AssetManager::GetTexture(const std::string& textureName)
    {
		auto texItr = m_mapTextures.find(textureName);
        if (texItr == m_mapTextures.end())
        {
            SPARK_ERROR("Failed to get texture [{0}] -- Does not exist!", textureName);
			return nullptr;
        }
        
        return texItr->second;
    }

    bool AssetManager::AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
    {
		// Check if shader already exists
        if (m_mapShaders.find(shaderName) != m_mapShaders.end())
        {
            SPARK_WARN("Failed to add shader [{0}] -- Already exists!", shaderName);
            return false;
        }

        // Create and load the shader
        auto shader = std::move(SPARK_RENDERING::ShaderLoader::Create(vertexPath, fragmentPath));

        if (!shader)
        {
            SPARK_ERROR("Failed to load shader [{0}] at vert paths [{1}] and frag path [{2}]", shaderName, vertexPath, fragmentPath);
            return false;
		}

		m_mapShaders.emplace(shaderName, std::move(shader));
        return true;
    }

    std::shared_ptr<SPARK_RENDERING::Shader> AssetManager::GetShader(const std::string& shaderName)
    {
        auto shaderItr = m_mapShaders.find(shaderName);
        if (shaderItr == m_mapShaders.end())
        {
            SPARK_ERROR("Failed to get shader [{0}] -- Does not exist!", shaderName);
            return nullptr;
        }

        return shaderItr->second;
    }

    void AssetManager::CreateLuaAssetManager(sol::state& lua, SPARK_CORE::ECS::Registry& registry)
    {
        auto& asset_manager = registry.GetContext<std::shared_ptr<AssetManager>>();
        if (!asset_manager)
        {
            SPARK_ERROR("Failed to bind the asset manager to lua - Does not exist in the registry!");
            return;
        }

        lua.new_usertype<AssetManager>(
            "AssetManager",
            sol::no_constructor,
            "add_texture", [&](const std::string assetName, const std::string& filepath, bool pixel_art)
            {
                return asset_manager->AddTexture(assetName, filepath, pixel_art);
            }
        );
    }

}