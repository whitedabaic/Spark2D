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

    bool AssetManager::AddMusic(const std::string& musicName, const std::string& filepath)
    {
        if (m_mapMusic.find(musicName) != m_mapMusic.end())
        {
            SPARK_ERROR("Failed to add music [{}] -- Aleady exists!", musicName);
            return false;
        }

        Mix_Music* music = Mix_LoadMUS(filepath.c_str());

        if (!music)
        {
            std::string error{ Mix_GetError() };
            SPARK_ERROR("Failed to load [{}] at path [{}] -- Mixer Error: {}", musicName, filepath, error);
            return false;
        }

        // Create the sound parameters
        SPARK_SOUNDS::SoundParams params{
            .name = musicName,
            .filename = filepath,
            .duration = Mix_MusicDuration(music)
        };

        // Create the music Pointer
        auto musicPtr = std::make_shared<SPARK_SOUNDS::Music>(params, MusicPtr{ music });
        if (!musicPtr)
        {
            SPARK_ERROR("Failed to create the music ptr for [{}]", musicName);
            return false;
        }
        m_mapMusic.emplace(musicName, std::move(musicPtr));

        return true;
    }

    std::shared_ptr<SPARK_SOUNDS::Music> AssetManager::GetMusic(const std::string& musicName)
    {
        auto musicItr = m_mapMusic.find(musicName);
        if (musicItr == m_mapMusic.end())
        {
            SPARK_ERROR("Failed to get [{}] -- Does not exist!", musicName);
            return nullptr;
        }

        return musicItr->second;
    }

    bool AssetManager::AddSoundFx(const std::string& soundFxName, const std::string& filepath)
    {
        if (m_mapSoundFx.find(soundFxName) != m_mapSoundFx.end())
        {
            SPARK_ERROR("Failed to add soundfx [{}] -- Already exists!", soundFxName);
            return false;
        }

        Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());

        if (!chunk)
        {
            std::string error{ Mix_GetError() };
            SPARK_ERROR("Failed to load [{}] at path [{}] -- Error: {}", soundFxName, filepath, error);
            return false;
        }

        SPARK_SOUNDS::SoundParams params{
            .name = soundFxName,
            .filename = filepath,
            .duration = chunk->alen / 179.4
        };

        auto pSoundFx = std::make_shared<SPARK_SOUNDS::SoundFX>(params, SoundFxPtr{ chunk });
        m_mapSoundFx.emplace(soundFxName, std::move(pSoundFx));

        return true;
    }

    std::shared_ptr<SPARK_SOUNDS::SoundFX> AssetManager::GetSoundFx(const std::string& soundFxName)
    {
        auto soundItr = m_mapSoundFx.find(soundFxName);
        if (soundItr == m_mapSoundFx.end())
        {
            SPARK_ERROR("Failed to get SoundFX [{}] -- Does Not exist!", soundFxName);
            return nullptr;
        }

        return soundItr->second;
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
            },
            "add_music", [&](const std::string& musicName, const std::string& filepath)
            {
                return asset_manager->AddMusic(musicName, filepath);
            },
            "add_soundfx", [&](const std::string& soundFxName, const std::string& filepath)
            {
                return asset_manager->AddSoundFx(soundFxName, filepath);
            }
        );
    }

}