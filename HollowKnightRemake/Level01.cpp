#include "pch.h"
#include "Level01.h"
#include "Map.h"
#include "GameObjectManager.h"
#include "Player.h"
#include "Collider.h"
#include "LevelManager.h"
#include "AudioManager.h"

Level01::Level01(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager):
	Level(map,levelManager,gameObjectManager)
{
}

void Level01::Update(float elpasedSec)
{
	Level::Update(elpasedSec);
}

void Level01::Draw() const
{
	Level::Draw();
}

void Level01::Load(GameObjectManager* gameObjectManager,EnemyManager* enemyManager, const std::string& parameter)
{
	Level::Load(gameObjectManager, enemyManager, parameter);

	std::string spawnTag{ "spawn" };
	if (parameter == "reverse")
	{
		spawnTag = "spawn2";
	}

	Player* player{ new Player() };
	player->SetPosition(gameObjectManager->GetObjectBasedOfTag(spawnTag)->GetPosition());
	gameObjectManager->AddObject(player);

	

	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("levelfinish") };
	Collider* collider{ triggerObject->GetComponent<Collider>() };
	
	collider->m_OnCollisions.push_back([this](Collider* collider) 
		{
			m_LevelManager->LoadLevel(level_02);
		}
	);
}

void Level01::Deload()
{
	Level::Deload();
}
