#pragma once
#include <vector>
#include <list>
#include <string>
#include "json.hpp"

using namespace nlohmann;
enum MapBuilderFilters;

class MapObject;
class TextureManager;
class Map final
{
public:
	Map();
	Map(const json& j, TextureManager* textureManager);
	~Map();

	// Editing utils
	void AddObject(MapObject* mapObject);
	void RemoveObject(const MapObject* mapObject);
	MapObject* GetObjectOnPosition(const Point2f& position, std::vector<MapBuilderFilters> filter);
	void CheckOrder(const MapObject* changedObject);

	// Runtime utils
	bool DoesObjectExist(int index);
	MapObject* FindObject(int index);
	std::vector<MapObject*> GetObjects();

	std::string GetName();

	// General
	void Draw(std::vector<MapBuilderFilters> filter);

	void ToJson(json& j);

	void SetName(const std::string& name);
private:
	void Sort();
	int CompareObjects(MapObject* first, MapObject* second);
	int GetValidId();
	bool DoesFilterContain(std::vector<MapBuilderFilters> filter, MapBuilderFilters filtertype);

	std::string m_Name;

	std::vector<MapObject*> m_Objects;
};

