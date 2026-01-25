#include "MusicPlayer.h"
#include "../Essentials/Music.h"
#include <Logger/Logger.h>

namespace SPARK_SOUNDS {
	MusicPlayer::MusicPlayer()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			std::string error{ Mix_GetError() };
			SPARK_ERROR("Unable to open SDL Music Mixer - {}", error);
			return;
		}
		auto channels = Mix_AllocateChannels(16);
		SPARK_LOG("CHANNELS ALLOCATED [{}]", channels);
	}

	MusicPlayer::~MusicPlayer()
	{
		Mix_HaltMusic();
		Mix_Quit();
		SPARK_LOG("Music Player Closed!");
	}

	void MusicPlayer::Play(Music& music, int loops)
	{
		if (!music.GetMusicPtr())
		{
			auto name = music.GetName();
			SPARK_ERROR("Failed to play music [{}] - Mix music was null",name);
			return;
		}

		if (Mix_PlayMusic(music.GetMusicPtr(), loops) != 0)
		{
			std::string error{ Mix_GetError() };
			auto name = music.GetName();
			SPARK_ERROR("Failed to play music [{}] Mix Error - {}", name, error);
		}
	}

	void MusicPlayer::Pause()
	{
		Mix_PauseMusic();
	}

	void MusicPlayer::Resume()
	{
		Mix_ResumeMusic();
	}

	void MusicPlayer::Stop()
	{
		Mix_HaltMusic();
	}

	void MusicPlayer::SetVolume(int volume)
	{
		if (volume < 0 || volume > 100)
		{
			SPARK_ERROR("Failed to set volume. Must be between 0 - 100 -- Input [{}]", volume);
			return;
		}

		// Scale the volume from 0 - 100%
		int volume_changed = static_cast<int>((volume / 100.f) * 128);
		Mix_VolumeMusic(volume_changed);
	}

	bool MusicPlayer::IsPlaying()
	{
		return Mix_PlayingMusic();
	}
}