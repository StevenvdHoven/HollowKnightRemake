#pragma once
#include <map>
#include "Enemy.h"

enum EnemyType 
{
	enemy_01,
	enemy_02,
	enemy_03,
	enemy_04,
	enemy_05,
	enemy_06,
	enemy_07,
	enemy_08
};

class GameObjectManager;
class EnemyManager final
{
public:
	EnemyManager();
	~EnemyManager();

	void CheckForSpawn(GameObjectManager* manager);

	GameObject* GetEnemy(EnemyType type);
	GameObject* GetEnemy(const std::string& tag);

private:
	void SpawnInVector(std::vector<GameObject*> foundObjects);

	GameObjectManager* m_GameObjectManager;
};

