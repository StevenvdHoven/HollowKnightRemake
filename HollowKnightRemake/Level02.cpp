#include "pch.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "LevelManager.h"
#include "Player.h"
#include "Level02.h"

Level02::Level02(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager):
	Level(map,levelManager,gameObjectManager)
{
}

void Level02::Update(float elpasedSec)
{
}

void Level02::Draw() const
{
}

void Level02::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
{
	Level::Load(gameObjectManager,enemyManager, parameter);

	std::string spawnTag{ "spawn" };
	if (parameter == "reverse")
	{
		spawnTag = "spawn2";
	}

	Player* player{ new Player() };
	player->SetPosition(gameObjectManager->GetObjectBasedOfTag(spawnTag)->GetPosition());
	gameObjectManager->AddObject(player);

	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("level03") };
	Collider* level03Collider{ triggerObject->GetComponent<Collider>() };

	level03Collider->m_OnCollisions.push_back([this, level03Collider](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			level03Collider->SetEnabled(false);
			m_LevelManager->LoadLevel(level_03);
			
		}
	);

	triggerObject = gameObjectManager->GetObjectBasedOfTag("level01");
	Collider* collider = triggerObject->GetComponent<Collider>();
	collider->m_OnCollisions.push_back([this](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			m_LevelManager->LoadLevel(level_01, "reverse");
		}
	);
}

void Level02::Deload()
{
	Level::Deload();
}
