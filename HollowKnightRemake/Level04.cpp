#include "pch.h"
#include "Level04.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "LevelManager.h"

Level04::Level04(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager) :
	Level(map, levelManager, gameObjectManager)
{
}

void Level04::Update(float elpasedSec)
{
	Level::Update(elpasedSec);
}

void Level04::Draw() const
{
	Level::Draw();
}

void Level04::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
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

	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("level06") };
	Collider* collider{ triggerObject->GetComponent<Collider>() };

	collider->m_OnCollisions.push_back([this, triggerObject](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			m_LevelManager->LoadLevel(level_06);
			m_GameObjectManager->DeleteObject(triggerObject);
		}
	);

	GameObject* returnTriggerObject{ gameObjectManager->GetObjectBasedOfTag("level03") };
	collider = returnTriggerObject->GetComponent<Collider>();

	collider->m_OnCollisions.push_back([this, returnTriggerObject](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			m_LevelManager->LoadLevel(level_03, "reverse");
			m_GameObjectManager->DeleteObject(returnTriggerObject);
		}
	);
}

void Level04::Deload()
{
	Level::Deload();
}