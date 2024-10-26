#include "pch.h"
#include "Level.h"
#include "Map.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "DestructableWeed.h"
#include "DestructableBarrel.h"
#include "DestructableCar.h"
#include "DestructableSpear.h"
#include "DestructableObject.h"
#include "DestructableGeo.h"
#include "Player.h"
#include "EnemyManager.h"
#include "WorldFireLightParticles.h"

Level::Level(Map* map, LevelManager* levelManager, GameObjectManager* gameObjectManager) :
	m_Map{ map },
	m_GameObjectManager{ gameObjectManager },
	m_LevelManager{ levelManager }
{
}

Level::~Level()
{
	m_Map = nullptr;
	m_GameObjectManager = nullptr;
	m_LevelManager = nullptr;
}

void Level::Update(float elapsedSec)
{
}

void Level::Draw() const
{

}

void Level::Load(GameObjectManager* gameObjectManager, EnemyManager* enemyManager, const std::string& parameter)
{
	std::vector<MapObject*> mapObjects{ m_Map->GetObjects() };
	for (int index{ 0 }; index < mapObjects.size(); ++index) {
		GameObject* gameObject{ new GameObject(mapObjects[index]) };
		gameObjectManager->AddObject(gameObject);
		gameObject->Start();
	}

	CheckForDistructableObjects();

	std::vector<GameObject*> worldParticleTemps{ gameObjectManager->GetObjectsBasedOfTag("worldparticle") };
	for (int index{ 0 }; index < worldParticleTemps.size(); ++index)
	{
		GameObject* worldParticle{ new WorldFireLightParticles() };
		worldParticle->SetPosition(worldParticleTemps[index]->GetPosition());
		gameObjectManager->AddObject(worldParticle);
	}

	enemyManager->CheckForSpawn(m_GameObjectManager);
}

void Level::Deload()
{
	m_GameObjectManager->Deload();
}

void Level::CheckForDistructableObjects()
{
	SpawnInDistructableObjects(m_GameObjectManager->GetObjectsBasedOfTag(destructable_weed));
	SpawnInDistructableObjects(m_GameObjectManager->GetObjectsBasedOfTag(destructable_barrel));
	SpawnInDistructableObjects(m_GameObjectManager->GetObjectsBasedOfTag(destructable_car));
	SpawnInDistructableObjects(m_GameObjectManager->GetObjectsBasedOfTag(destructable_spear));
	SpawnInDistructableObjects(m_GameObjectManager->GetObjectsBasedOfTag(destructable_geo));
}

void Level::SpawnInDistructableObjects(std::vector<GameObject*> taggedObjects)
{
	for (int index{ 0 }; index < taggedObjects.size(); ++index)
	{
		GameObject* gameObject{ GetTaggedDistructableObject(taggedObjects[index]->m_Tag) };
		m_GameObjectManager->AddObject(gameObject);
		gameObject->m_Transform = taggedObjects[index]->m_Transform;

		DestructableObject* destructable{ gameObject->GetComponent<DestructableObject>() };
		destructable->Link(taggedObjects[index]);
	}
}

GameObject* Level::GetTaggedDistructableObject(const std::string& tag)
{
	desturctableObjects convertedTag{ Weed };
	if (tag == destructable_weed) convertedTag = Weed;
	else if (tag == destructable_barrel) convertedTag = Barrel;
	else if (tag == destructable_car) convertedTag = Car;
	else if (tag == destructable_spear) convertedTag = Spear;
	else if (tag == destructable_geo) convertedTag = Geo;

	switch (convertedTag)
	{
	case Weed:
		return new DestructableWeed();
	case Barrel:
		return new DestructableBarrel();
	case Car:
		return new DestructableCar();
	case Spear:
		return new DestructableSpear();
	case Geo:
		return new DestructableGeo();
	}
	return nullptr;
}

