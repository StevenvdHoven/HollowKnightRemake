#pragma once
#include <vector>

#define destructable_weed	"destructweed"
#define destructable_barrel "destructbarrel" 
#define destructable_car	"destructcar" 
#define destructable_spear	"destructspear" 
#define destructable_geo	"destructgeo" 

enum desturctableObjects
{
	Weed,
	Barrel,
	Car,
	Spear,
	Geo
};

class Map;
class GameObjectManager;
class LevelManager;
class GameObject;
class EnemyManager;
class Level
{
public:
	Level(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager);
	virtual ~Level();

	virtual void Update(float elpasedSec);
	virtual void Draw() const;

	virtual void Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter = "");
	virtual void Deload();

private:
	void CheckForDistructableObjects();
	void SpawnInDistructableObjects(std::vector<GameObject*> taggedObjects);
	GameObject* GetTaggedDistructableObject(const std::string& tag);

	Map* m_Map;

protected:
	GameObjectManager* m_GameObjectManager;
	LevelManager* m_LevelManager;
};

