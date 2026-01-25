#include "SoundFxPlayer.h"
#include "../Essentials/SoundFX.h"
#include <Logger/Logger.h>

namespace SPARK_SOUNDS {

	void SoundFxPlayer::Play(SoundFX& soundFx)
	{
		Play(soundFx, 0, -1);
	}

	void SoundFxPlayer::Play(SoundFX& soundFx, int loops, int channel)
	{
		if (!soundFx.GetSoundFxPtr())
		{
			auto name = soundFx.GetName();
			SPARK_ERROR("Failed to play SoundFX[{}] on channel [{}] -- SoundFX* is null", name, channel);
			return;
		}

		if (Mix_PlayChannel(channel, soundFx.GetSoundFxPtr(), loops) == -1)
		{
			std::string error{ Mix_GetError() };
			auto name = soundFx.GetName();
			SPARK_ERROR("Failed to play SoundFX[{}] on channel[{}] -- ERROR: {}",name ,channel, error);
		}
	}

	void SoundFxPlayer::SetVolume(int channel, int volume)
	{
		if (volume < 0 || volume > 100)
		{
			SPARK_ERROR("Failed to set volume. Must be between 0 - 100 -- Input [{}]", volume);
			return;
		}

		// Scale the volume from 0 - 100%
		int volume_changed = static_cast<int>((volume / 100.f) * 128);
		Mix_Volume(channel, volume_changed);
	}

	void SoundFxPlayer::Stop(int channel)
	{
		if (Mix_HaltChannel(channel) == -1)
		{
			std::string error{ Mix_GetError() };
			auto e = channel == -1 ? "all channels" : std::to_string(channel);
			SPARK_ERROR("Failed to halt soundfx for channel [{}]", e);
		}
	}

	bool SoundFxPlayer::IsPlaying(int channel)
	{
		return Mix_Playing(channel);
	}

}