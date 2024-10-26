#include "pch.h"
#include "Level09.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "LevelManager.h"


Level09::Level09(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager) :
	Level(map, levelManager, gameObjectManager)
{
}

void Level09::Update(float elpasedSec)
{
}

void Level09::Draw() const
{
}

void Level09::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
{
	Level::Load(gameObjectManager, enemyManager, parameter);

	std::string spawnTag{ "spawn" };
	Player* player{ new Player() };
	player->SetPosition(gameObjectManager->GetObjectBasedOfTag(spawnTag)->GetPosition());
	gameObjectManager->AddObject(player);

	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("level08") };
	Collider* level08Collider{ triggerObject->GetComponent<Collider>() };

	level08Collider->m_OnCollisions.push_back([this, level08Collider](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			level08Collider->SetEnabled(false);
			m_LevelManager->LoadLevel(level_08);

		}
	);

}

void Level09::Deload()
{
	Level::Deload();
}