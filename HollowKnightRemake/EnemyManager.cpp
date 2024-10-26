#include "pch.h"
#include "EnemyManager.h"
#include "GameObjectManager.h"
#include "Crawlid.h"
#include "Gruzzer.h"
#include "TikTik.h"
#include "HuskBully.h"
#include "AspidHunter.h"
#include "Goam.h"
#include "Vengefly.h"

EnemyManager::EnemyManager():
    m_GameObjectManager{nullptr}
{
}

EnemyManager::~EnemyManager()
{
    m_GameObjectManager = nullptr;
}

void EnemyManager::CheckForSpawn(GameObjectManager* manager)
{
    if (m_GameObjectManager == nullptr) {
        m_GameObjectManager = manager;
    }

    std::vector<GameObject*> foundEnemy01Objects{ m_GameObjectManager->GetObjectsBasedOfTag("enemy01") };
    std::vector<GameObject*> foundEnemy02Objects{ m_GameObjectManager->GetObjectsBasedOfTag("enemy02") };
    std::vector<GameObject*> foundEnemy03Objects{ m_GameObjectManager->GetObjectsBasedOfTag("enemy03") };
    std::vector<GameObject*> foundEnemy04Objects{ m_GameObjectManager->GetObjectsBasedOfTag("enemy04") };
    std::vector<GameObject*> foundEnemy05Objects{ m_GameObjectManager->GetObjectsBasedOfTag("enemy05") };
    std::vector<GameObject*> foundEnemy06Objects{ m_GameObjectManager->GetObjectsBasedOfTag("enemy06") };
    std::vector<GameObject*> foundEnemy07Objects{ m_GameObjectManager->GetObjectsBasedOfTag("enemy07") };
    std::vector<GameObject*> foundEnemy08Objects{ m_GameObjectManager->GetObjectsBasedOfTag("enemy08") };

    SpawnInVector(foundEnemy01Objects);
    SpawnInVector(foundEnemy02Objects);
    SpawnInVector(foundEnemy03Objects);
    SpawnInVector(foundEnemy04Objects);
    SpawnInVector(foundEnemy05Objects);
    SpawnInVector(foundEnemy06Objects);
    SpawnInVector(foundEnemy07Objects);
    SpawnInVector(foundEnemy08Objects);
}

GameObject* EnemyManager::GetEnemy(EnemyType type)
{
    switch (type
        )
    {
    case enemy_01:
        return new Crawlid();
    case enemy_02:
        return new TikTik();
    case enemy_03:
        return new Gruzzer();
    case enemy_04:
        return new AspidHunter();
    case enemy_05:
        return new HuskBully();
    case enemy_06:
        return new Goam();
    case enemy_07:
        return new Vengefly();
    case enemy_08:
        break;
    }
    return nullptr;
}

GameObject* EnemyManager::GetEnemy(const std::string& tag)
{
    std::map<std::string, EnemyType> types{
        {"enemy01",enemy_01},
        {"enemy02",enemy_02},
        {"enemy03",enemy_03},
        {"enemy04",enemy_04},
        {"enemy05",enemy_05},
        {"enemy06",enemy_06},
        {"enemy07",enemy_07},
        {"enemy08",enemy_08},
    };

    EnemyType type{ types[tag] };

    return GetEnemy(type);
}

void EnemyManager::SpawnInVector(std::vector<GameObject*> foundObjects)
{
    for (int index{ 0 }; index < foundObjects.size(); ++index) {
        GameObject* spawn{ GetEnemy(foundObjects[index]->m_Tag) };
        spawn->SetPosition(foundObjects[index]->GetPosition());
        spawn->m_Transform.Rotation = foundObjects[index]->m_Transform.Rotation;
        spawn->m_Transform.Scale = foundObjects[index]->m_Transform.Scale;
        m_GameObjectManager->AddObject(spawn);
    }
}
