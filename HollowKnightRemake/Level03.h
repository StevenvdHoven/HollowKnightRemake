#pragma once
#include "Level.h"
#include <vector>
#include "SoundEffect.h"

class Level03 final : public Level
{
public:
	Level03(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager);

	void Update(float elpasedSec) override;
	void Draw() const override;

	void Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter = "") override;
	void Deload() override;

private:
	void SetUpLevelTranstions(GameObjectManager* gameObjectManager);
	void SetUpCages(GameObjectManager* gameObjectManager);
	void SpawnCornifer(GameObjectManager* gameObjectManager);
	void SpawnEnemies(GameObjectManager* gameObjectManager, EnemyManager* enemyManager);



	std::vector<GameObject*> m_LinkedCages;
	std::vector<Point2f> m_StartPositions;
	SoundEffect* m_CloseSoundEffect;
	float m_ClosePercentage;
	bool m_Closed;

	std::vector<GameObject*> m_EnemySpawns;
	int m_KillCount;

	Collider* m_CageTrigger;
};

