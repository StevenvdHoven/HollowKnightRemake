#include "pch.h"
#include "Level06.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "LevelManager.h"


Level06::Level06(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager) :
	Level(map, levelManager, gameObjectManager)
{
}

void Level06::Update(float elpasedSec)
{
}

void Level06::Draw() const
{
}

void Level06::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
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

	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("level04") };
	Collider* level04Collider{ triggerObject->GetComponent<Collider>() };

	level04Collider->m_OnCollisions.push_back([this, level04Collider](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			level04Collider->SetEnabled(false);
			m_LevelManager->LoadLevel(level_04, "reverse2");

		}
	);

	triggerObject = gameObjectManager->GetObjectBasedOfTag("level07");
	Collider* level07Collider{ triggerObject->GetComponent<Collider>() };

	level07Collider->m_OnCollisions.push_back([this, level07Collider](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			level07Collider->SetEnabled(false);
			m_LevelManager->LoadLevel(menu);

		}
	);

}

void Level06::Deload()
{
	Level::Deload();
}