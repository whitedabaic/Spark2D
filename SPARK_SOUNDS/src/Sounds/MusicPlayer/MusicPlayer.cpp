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
			return;
		}
	}

	void MusicPlayer::Pause()
	{
	}

	void MusicPlayer::Resume()
	{
	}

	void MusicPlayer::Stop()
	{
	}
	void MusicPlayer::SetVolume(int volume)
	{
	}

	bool MusicPlayer::IsPlaying()
	{
		return false;
	}
}