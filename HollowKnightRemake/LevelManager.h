#pragma once
#include <map>
#include "Level.h"
#include "MapBuilder.h"
#include "GUIShape.h"
#include "Curve.h"

#define menu "menu"
#define level_01 "level01"
#define level_02 "level02"
#define level_03 "level03"
#define level_04 "level04"
#define level_05 "level05"
#define level_06 "level06"
#define level_07 "level07"
#define level_08 "level08"
#define level_09 "level09"
 
class GameObjectManager;
class EnemyManager;
class LevelManager final
{
public:
	LevelManager();
	~LevelManager();

	void DeleteLevels();
	void CreateLevels(const MapBuilder* mapBuilder, GameObjectManager* gameObjectManager, EnemyManager* enemyManager);
	void Update(float elapsedSec);
	void Draw();
	void LoadLevel(const std::string& name,std::string parameter = "", float timer = 1.5f);

	std::string GetCurrentLevel();

private:
	void LoadLevel();

	Level* m_CurrentLevel;
	std::map<std::string, Level*> m_AllLevels;

	GameObjectManager* m_GameObjectManager;
	EnemyManager* m_EnemyManager;

	std::string m_NewLevelName;
	std::string m_NewLevelParameter;
	bool m_LoadNewLevel;
	float m_LevelTimer;
	float m_MaxTime;

	GUIShape* m_Transition;

	Curve m_TransitionCurve;
};

