#include "pch.h"
#include "Level07.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "LevelManager.h"


Level07::Level07(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager) :
	Level(map, levelManager, gameObjectManager)
{
}

void Level07::Update(float elpasedSec)
{
}

void Level07::Draw() const
{
}

void Level07::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
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

	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("level08") };
	Collider* level04Collider{ triggerObject->GetComponent<Collider>() };

	level04Collider->m_OnCollisions.push_back([this, level04Collider](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			level04Collider->SetEnabled(false);
			m_LevelManager->LoadLevel(menu);

		}
	);


	triggerObject = gameObjectManager->GetObjectBasedOfTag("level06");
	Collider* level06Collider{ triggerObject->GetComponent<Collider>() };

	level06Collider->m_OnCollisions.push_back([this, level06Collider](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			level06Collider->SetEnabled(false);
			m_LevelManager->LoadLevel(level_06, "reverse");

		}
	);
}

void Level07::Deload()
{
	Level::Deload();
}
