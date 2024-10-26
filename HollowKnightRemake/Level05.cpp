#include "pch.h"
#include "Level05.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "LevelManager.h"


Level05::Level05(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager) :
	Level(map, levelManager, gameObjectManager)
{
}

void Level05::Update(float elpasedSec)
{
}

void Level05::Draw() const
{
}

void Level05::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
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

	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("level03") };
	Collider* level03Collider{ triggerObject->GetComponent<Collider>() };

	level03Collider->m_OnCollisions.push_back([this, level03Collider](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			level03Collider->SetEnabled(false);
			m_LevelManager->LoadLevel(level_03,"reverse2");

		}
	);

}

void Level05::Deload()
{
	Level::Deload();
}
