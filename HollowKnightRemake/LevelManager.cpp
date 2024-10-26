#include "pch.h"
#include "LevelManager.h"
#include "GameObject.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "EnemyManager.h"
#include "PlayerStats.h"
#include "GUIManager.h"
#include "SoundEffect.h"
#include "Menu.h"
#include "Level01.h"
#include "Level02.h"
#include "Level03.h"
#include "Level04.h"
#include "Level05.h"
#include "Level06.h"
#include "Level07.h"
#include "Level08.h"
#include "Level09.h"

LevelManager::LevelManager() :
	m_CurrentLevel{ nullptr },
	m_AllLevels{},
	m_GameObjectManager{ nullptr },
	m_LevelTimer{ -1 },
	m_MaxTime{ -1 },
	m_TransitionCurve{ Curve{Vector2f{0,0},Vector2f{0.1,1.36},Vector2f{0.9,1.36},Vector2f{1,0}}}
{
}

LevelManager::~LevelManager()
{
	DeleteLevels();
	m_GameObjectManager = nullptr;
}

void LevelManager::DeleteLevels()
{
	if (m_AllLevels.size() <= 0) {
		return;
	}
	for (std::map<std::string, Level*>::iterator it = m_AllLevels.begin(); it != m_AllLevels.end(); ++it)
	{
		delete it->second;
	}
	m_AllLevels.clear();
	m_CurrentLevel = nullptr;
}

void LevelManager::CreateLevels(const MapBuilder* mapBuilder, GameObjectManager* gameObjectManager, EnemyManager* enemyManager)
{
	m_GameObjectManager = gameObjectManager;
	m_EnemyManager = enemyManager;

	m_AllLevels[menu] = new Menu(mapBuilder->GetMap(menu), this, gameObjectManager);
	m_AllLevels[level_01] = new Level01(mapBuilder->GetMap(level_01), this, gameObjectManager);
	m_AllLevels[level_02] = new Level02(mapBuilder->GetMap(level_02), this, gameObjectManager);
	m_AllLevels[level_03] = new Level03(mapBuilder->GetMap(level_03), this, gameObjectManager);
	m_AllLevels[level_04] = new Level04(mapBuilder->GetMap(level_04), this, gameObjectManager);
	m_AllLevels[level_05] = new Level05(mapBuilder->GetMap(level_05), this, gameObjectManager);
	m_AllLevels[level_06] = new Level06(mapBuilder->GetMap(level_06), this, gameObjectManager);
	m_AllLevels[level_07] = new Level07(mapBuilder->GetMap(level_07), this, gameObjectManager);
	m_AllLevels[level_08] = new Level08(mapBuilder->GetMap(level_08), this, gameObjectManager);
	m_AllLevels[level_09] = new Level09(mapBuilder->GetMap(level_09), this, gameObjectManager);

	m_CurrentLevel = m_AllLevels[level_01];


	m_Transition = new GUIShape
	{
		Point2f{1920 * 0.5f,1080 * 0.5f},
		std::vector<Point2f>
		{
			Point2f{-1920 * 0.5f,1080 * 0.5f},
			Point2f{1920 * 0.5f,1080 * 0.5f},
			Point2f{1920 * 0.5f,-1080 * 0.5f},
			Point2f{-1920 * 0.5f,-1080 * 0.5f},
		},
		Color4f{0,0,0,1}
	};

	gameObjectManager->GetGUIManager()->AddGUI(m_Transition);
	m_Transition->SetOrder(100);
}

void LevelManager::Update(float elapsedSec)
{
	m_CurrentLevel->Update(elapsedSec);
	if (m_LoadNewLevel) 
	{
		if (m_LevelTimer > 0)
		{
			m_LevelTimer -= elapsedSec;

			float percentile{ 1 - (m_LevelTimer / m_MaxTime) };
			float opacity{ m_TransitionCurve.GetValue((percentile))};
			if (opacity > 0.95f)
			{
				opacity = 1.f;
			}
				
			
			m_Transition->SetOpacity(opacity);

			if (opacity > 0.95f)
			{
				LoadLevel();
			}
		}
		else
		{
			m_LoadNewLevel = false;
			m_Transition->SetHiding(true);
		}
		
	}
}

void LevelManager::Draw()
{
	m_CurrentLevel->Draw();
}

void LevelManager::LoadLevel(const std::string& name, std::string parameter, float timer)
{
	m_LoadNewLevel = true;
	m_NewLevelName = name;
	m_NewLevelParameter = parameter;

	m_LevelTimer = timer;
	m_MaxTime = timer;

	m_Transition->SetHiding(false);
}

std::string LevelManager::GetCurrentLevel()
{
	return m_NewLevelName;
}

void LevelManager::LoadLevel()
{
	GameObject* playerObject{ m_GameObjectManager->GetObjectBasedOfTag("player") };
	if (playerObject != nullptr)
	{
		PlayerStats* stats{ playerObject->GetComponent<PlayerStats>() };
		if (stats != nullptr)
		{
			stats->Save();
		}
	}
	

	if (m_CurrentLevel != nullptr) {
		m_CurrentLevel->Deload();
	}

	m_CurrentLevel = m_AllLevels[m_NewLevelName];
	m_CurrentLevel->Load(m_GameObjectManager,m_EnemyManager,m_NewLevelParameter);

	
	m_GameObjectManager->StartAll();

	SoundEffect::StopAll();
	
}
