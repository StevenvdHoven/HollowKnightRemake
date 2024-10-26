#include "pch.h"
#include "Collider.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "LevelManager.h"
#include "Cornifer.h"
#include "Player.h"
#include "Level03.h"
#include "AspidHunter.h"
#include "Damageable.h"
#include "AudioManager.h"

Level03::Level03(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager):
	Level(map,levelManager,gameObjectManager),
	m_ClosePercentage{ 0 },
	m_Closed{ false },
	m_KillCount{2}
{
}

void Level03::Update(float elpasedSec)
{
	Level::Update(elpasedSec);

	if (m_Closed)
	{
		if (m_ClosePercentage > 0)
		{
			m_ClosePercentage -= elpasedSec;
		}
		else
		{
			m_ClosePercentage = 0;
		}
	}
	else
	{
		if (m_ClosePercentage < 1)
		{
			m_ClosePercentage += elpasedSec;
		}
		else
		{
			m_ClosePercentage = 1;
		}
	}

	for (int index{ 0 }; index < m_LinkedCages.size(); ++index)
	{
		m_LinkedCages[index]->SetPosition(m_StartPositions[index] + Vector2f{0,200} * m_ClosePercentage, true);
	}
}

void Level03::Draw() const
{
	Level::Draw();
}

void Level03::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
{
	Level::Load(gameObjectManager, enemyManager, parameter);

	std::string spawnTag{ "spawn" };
	if (parameter == "reverse")
	{
		spawnTag = "spawn2";
	}
	else if (parameter == "reverse2")
	{
		spawnTag = "spawn3";
	}

	Player* player{ new Player() };
	player->SetPosition(gameObjectManager->GetObjectBasedOfTag(spawnTag)->GetPosition());
	gameObjectManager->AddObject(player);

	SetUpLevelTranstions(gameObjectManager);

	SetUpCages(gameObjectManager);

	SpawnCornifer(gameObjectManager);

	SpawnEnemies(gameObjectManager,enemyManager);
}

void Level03::Deload()
{
	Level::Deload();
}

void Level03::SetUpLevelTranstions(GameObjectManager* gameObjectManager)
{
	GameObject* triggerObject{ gameObjectManager->GetObjectBasedOfTag("level04") };
	Collider* collider{ triggerObject->GetComponent<Collider>() };

	collider->m_OnCollisions.push_back([this, triggerObject](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			m_LevelManager->LoadLevel(level_04);
			m_GameObjectManager->DeleteObject(triggerObject);
		}
	);

	GameObject* level05TriggerObject{ gameObjectManager->GetObjectBasedOfTag("level05") };
	collider = level05TriggerObject->GetComponent<Collider>();

	collider->m_OnCollisions.push_back([this, level05TriggerObject](Collider* collider)
		{
			if (collider->GetGameObject()->m_Tag != "player")
			{
				return;
			}
			m_LevelManager->LoadLevel(level_05);
			m_GameObjectManager->DeleteObject(level05TriggerObject);
		}
	);
}

void Level03::SetUpCages(GameObjectManager* gameObjectManager)
{
	m_CloseSoundEffect = m_GameObjectManager->GetAudioManager()->GetAudioClip("SFX/gate slam2.mp3");

	if (m_KillCount > 0)
	{
		m_KillCount = 2;

		GameObject* cageTriggerObject{ gameObjectManager->GetObjectBasedOfTag("gatetrigger") };
		m_CageTrigger = cageTriggerObject->GetComponent<Collider>();

		m_CageTrigger->m_OnCollisions.push_back([this](Collider* collider)
			{
				if (collider->GetGameObject()->m_Tag != "player")
				{
					return;
				}

				m_CageTrigger->SetEnabled(false);
				for (int index{ 0 }; index < m_LinkedCages.size(); ++index)
				{
					m_LinkedCages[index]->GetComponent<Collider>()->SetEnabled(true);
				}
				m_CloseSoundEffect->Play(0);
				m_Closed = true;
			}
		);
	}

	m_LinkedCages = gameObjectManager->GetObjectsBasedOfTag("gate");

	for (int index{ 0 }; index < m_LinkedCages.size(); ++index)
	{
		m_StartPositions.push_back(m_LinkedCages[index]->GetPosition());
		m_LinkedCages[index]->GetComponent<Collider>()->SetEnabled(false);
	}
}

void Level03::SpawnCornifer(GameObjectManager* gameObjectManager)
{
	GameObject* corniferSpawn{ gameObjectManager->GetObjectBasedOfTag("cornifer") };
	if (corniferSpawn != nullptr)
	{
		Cornifer* cornifer{ new Cornifer{} };
		cornifer->SetPosition(corniferSpawn->GetPosition(), true);
		gameObjectManager->AddObject(cornifer);
	}
}

void Level03::SpawnEnemies(GameObjectManager* gameObjectManager, EnemyManager* enemyManager)
{
	
	m_EnemySpawns = gameObjectManager->GetObjectsBasedOfTag("enemyspawn");
	AspidHunter* hunter{ new AspidHunter{} };
	hunter->SetPosition(m_EnemySpawns[0]->GetPosition());
	if (m_KillCount > 0)
	{
		hunter->GetComponent<Damageable>()->AddListenerOnDead
		(
			[this, enemyManager]()
			{
				for (int index{ 1 }; index < m_EnemySpawns.size(); ++index)
				{
					AspidHunter* newhunter{ new AspidHunter{} };
					newhunter->SetPosition(m_EnemySpawns[index]->GetPosition());
					newhunter->GetComponent<Damageable>()->AddListenerOnDead
					(
						[this]()
						{
							--m_KillCount;
							if (m_KillCount <= 0)
							{
								m_Closed = false;
								for (int index{ 0 }; index < m_LinkedCages.size(); ++index)
								{
									m_LinkedCages[index]->GetComponent<Collider>()->SetEnabled(false);
								}
							}
						}
					);
					m_GameObjectManager->AddObject(newhunter);
				}
			}
		);
	}
	gameObjectManager->AddObject(hunter);
}
