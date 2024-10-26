#include "pch.h"
#include "PlayerDataManager.h"
#include <fstream>

PlayerData::PlayerData(int health, int geo, float enery, const std::string& level, bool mapUnlocked, const std::string& respawnPoint):
    Health{health},
    Geo{geo},
    Energy{enery},
    CurrentLevel{level},
    HasMap{mapUnlocked},
    KillRespawnPoint{respawnPoint}
{
}

PlayerData::PlayerData(json& json):
    Health{ json["Health"].get<int>() },
    Geo{json["Geo"].get<int>()},
    Energy{json["Energy"].get<float>()},
    CurrentLevel{ json["Level"].get<std::string>() },
    HasMap{ json["MapUnlocked"].get<bool>() },
    KillRespawnPoint{ json["RespawnPoint"].get<std::string>()}
{
    
}

json PlayerData::GetJson() const
{
    return json::object(
        {
            {"Health",Health},
            {"Geo",Geo},
            {"Energy",Energy},
            {"Level",CurrentLevel},
            {"MapUnlocked",HasMap},
            {"RespawnPoint",KillRespawnPoint}
        });
}

PlayerDataManager::PlayerDataManager()
{
    SavePlayerData(PlayerData{});
}

void PlayerDataManager::SavePlayerData(const PlayerData& data)
{
    json j{ data.GetJson() };
    std::ofstream stream("player.json");
    stream << j.dump(4);
    stream.close();
}

PlayerData PlayerDataManager::LoadPlayerData()
{
    PlayerData data;

	std::fstream stream("player.json");
	if (stream.is_open() && stream.good()) {
		json j{ json::parse(stream) };

        data = PlayerData(j);

	}

    return data;
}
