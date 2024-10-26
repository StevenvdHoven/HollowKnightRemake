#pragma once
#include "json.hpp"

using namespace nlohmann;

struct PlayerData
{
	PlayerData(int health = 5, int geo = 0, float enery = 0.f,const std::string& level = "level01", bool mapUnlocked = false, const std::string& respawnPoint = "level01");
	PlayerData(json& json);

	int Health;
	int Geo;
	float Energy;
	std::string CurrentLevel;
	bool HasMap;
	std::string KillRespawnPoint;

	json GetJson() const;
};

class PlayerDataManager
{
public:
	PlayerDataManager();

	void SavePlayerData(const PlayerData& data);
	PlayerData LoadPlayerData();
};

