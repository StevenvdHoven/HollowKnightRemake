#pragma once
#include "SoundEffect.h"
#include "SoundStream.h"
#include <map>

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	void LoadAudioClips();
	SoundEffect* GetAudioClip(const std::string& filePath);
	SoundStream* GetAudioStream(const std::string& filePath);

	void StopAll();

private:
	std::map<std::string, SoundEffect*> m_AudioClips;
	std::map<std::string, SoundStream*> m_AudioStreams;
};

