#pragma once
#include "Level.h"
#include "SoundStream.h"

class Map;
class GameObjectManager;
class AudioManager;
class Level01 final : public Level
{
public:
	Level01(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager);

	void Update(float elpasedSec) override;
	void Draw() const override;

	void Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter = "") override;
	void Deload() override;

private:
	SoundStream* m_MainSoundTrack;
	SoundStream* m_MainBassTrack;
};

