#include "SoundBindings.h"
#include "../ECS/Registry.h"
#include"../Resources/AssetManager.h"
#include <Sounds/MusicPlayer/MusicPlayer.h>
#include <Logger/Logger.h>

using namespace SPARK_SOUNDS;
using namespace SPARK_RESOURCES;

void SPARK_CORE::Scripting::SoundBinder::CreateSoundBind(sol::state& lua, SPARK_CORE::ECS::Registry& registry)
{
	auto& musicPlayer = registry.GetContext<std::shared_ptr<MusicPlayer>>();
	if (!musicPlayer)
	{
		SPARK_ERROR("Failed to bind the music player to lua -- Not in the registry!");
		return;
	}

	auto& assetManager = registry.GetContext<std::shared_ptr<AssetManager>>();
	if (!assetManager)
	{
		SPARK_ERROR("Failed to bind the music player to lua -- Asset manager does not exist in registry!");
		return;
	}

	lua.new_usertype<MusicPlayer>(
		"Music",
		sol::no_constructor,
		"play", sol::overload(
			[&](const std::string& musicName, int loops) {
				auto music = assetManager->GetMusic(musicName);
				if (!music)
				{
					SPARK_ERROR("Failed to get music [{}] - From the asset manager!", musicName);
					return;
				}
				musicPlayer->Play(*music, loops);
			},
			[&](const std::string& musicName) {
				auto music = assetManager->GetMusic(musicName);
				if (!music)
				{
					SPARK_ERROR("Failed to get music [{}] - From the asset manager!", musicName);
					return;
				}
				musicPlayer->Play(*music, -1);
			}
		),
		"stop", [&]() {
			musicPlayer->Stop();
		},
		"pause", [&]() {
			musicPlayer->Pause();
		},
		"resume", [&]() {
			musicPlayer->Resume();
		},
		"set_volume", [&](int volume) {
			musicPlayer->SetVolume(volume);
		},
		"is_playing", [&]() {
			return musicPlayer->IsPlaying();
		}
	);
}
