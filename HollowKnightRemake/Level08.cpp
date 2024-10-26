#include "pch.h"
#include "Level08.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "LevelManager.h"


Level08::Level08(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager) :
	Level(map, levelManager, gameObjectManager)
{
}

void Level08::Update(float elpasedSec)
{
}

void Level08::Draw() const
{
}

void Level08::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
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

	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("menu") };
	Collider* menuCollider{ triggerObject->GetComponent<Collider>() };

	menuCollider->m_OnCollisions.push_back([this, menuCollider](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			menuCollider->SetEnabled(false);
			m_LevelManager->LoadLevel(menu);

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
			m_LevelManager->LoadLevel(level_07, "reverse");

		}
	);

}

void Level08::Deload()
{
	Level::Deload();
}
