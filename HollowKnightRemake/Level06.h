#pragma once
#include "Level.h"
class Level06 final : public Level
{
public:
	Level06(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager);

	void Update(float elpasedSec) override;
	void Draw() const override;

	void Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter = "") override;
	void Deload() override;

private:

};

