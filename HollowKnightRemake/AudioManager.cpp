#include "pch.h"
#include "AudioManager.h"
#include <iostream>
#include <vector>

AudioManager::AudioManager()
{
	LoadAudioClips();
}

AudioManager::~AudioManager()
{
	for (std::map<std::string, SoundEffect*>::iterator it = m_AudioClips.begin(); it != m_AudioClips.end(); ++it) 
	{
		delete it->second;
	}
	m_AudioClips.clear();

	for (std::map<std::string, SoundStream*>::iterator it = m_AudioStreams.begin(); it != m_AudioStreams.end(); ++it)
	{
		delete it->second;
	}
	m_AudioStreams.clear();
}

void AudioManager::LoadAudioClips()
{
	std::vector<std::string> commonsAudioFilePath
	{
		"Audio/Player/sword1.mp3",
	};

	for (int index{ 0 }; index < commonsAudioFilePath.size(); ++index)
	{
		m_AudioClips.emplace(commonsAudioFilePath[index], new SoundEffect(commonsAudioFilePath[index]));
	}
}

SoundEffect* AudioManager::GetAudioClip(const std::string& filePath)
{
	std::map<std::string, SoundEffect*>::const_iterator findResult{ m_AudioClips.find(filePath) };
	if (findResult != m_AudioClips.end()) {
		return m_AudioClips[filePath];
	}
	else {
		SoundEffect* sfx{ new SoundEffect("Audio/" + filePath)};
		if (sfx->IsLoaded()) {
			m_AudioClips.emplace(filePath, sfx);
			return sfx;
		}
		else {
			std::cout << "File wasn't loaded correctly FILEPATH: " << filePath << std::endl;
			return nullptr;
		}
	}
}

SoundStream* AudioManager::GetAudioStream(const std::string& filePath)
{
	std::map<std::string, SoundStream*>::const_iterator findResult{ m_AudioStreams.find(filePath) };
	if (findResult != m_AudioStreams.end()) {
		return m_AudioStreams[filePath];
	}
	else {
		SoundStream* sfx{ new SoundStream("Audio/" + filePath) };
		if (sfx->IsLoaded()) {
			m_AudioStreams.emplace(filePath, sfx);
			return sfx;
		}
		else {
			std::cout << "File wasn't loaded correctly FILEPATH: " << filePath << std::endl;
			return nullptr;
		}
	}
}

void AudioManager::StopAll()
{
	SoundEffect::StopAll();
	SoundStream::Stop();
}
