#pragma once
#include <map>
#include <memory>
#include <string>

#include <Rendering/Essentials//Shader.h>
#include <Rendering/Essentials/Texture.h>
#include <Rendering/Essentials/Font.h>

#include <Sounds/Essentials/Music.h>
#include <Sounds/Essentials/SoundFx.h>

#include "../ECS/Registry.h"
#include <sol/sol.hpp>

namespace SPARK_RESOURCES {
	class AssetManager
	{
	private:
		std::map<std::string, std::shared_ptr<SPARK_RENDERING::Texture>> m_mapTextures{};
		std::map<std::string, std::shared_ptr<SPARK_RENDERING::Shader>> m_mapShaders{};
		std::map<std::string, std::shared_ptr<SPARK_RENDERING::Font>> m_mapFonts{};

		std::map<std::string, std::shared_ptr<SPARK_SOUNDS::Music>> m_mapMusic{};
		std::map<std::string, std::shared_ptr<SPARK_SOUNDS::SoundFX>> m_mapSoundFx{};
	public:
		AssetManager() = default;
		~AssetManager() = default;

		bool AddTexture(const std::string& textureName, const std::string& texturePath, bool pixelArt = true);
		std::shared_ptr<SPARK_RENDERING::Texture> GetTexture(const std::string& textureName);

		bool AddFont(const std::string& fontName, const std::string& fontPath, float fontSize = 32.f);
		bool AddFontFromMemory(const std::string& fontName, unsigned char* fontData, float fontSize = 32.f);
		std::shared_ptr<SPARK_RENDERING::Font> GetFont(const std::string& fontName);

		bool AddShader(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		std::shared_ptr<SPARK_RENDERING::Shader> GetShader(const std::string& shaderName);

		bool AddMusic(const std::string& musicName, const std::string& filepath);
		std::shared_ptr<SPARK_SOUNDS::Music> GetMusic(const std::string& musicName);

		bool AddSoundFx(const std::string& soundFxName, const std::string& filepath);
		std::shared_ptr<SPARK_SOUNDS::SoundFX> GetSoundFx(const std::string& soundFxName);

		static void CreateLuaAssetManager(sol::state& lua, SPARK_CORE::ECS::Registry& registry);
	};
}