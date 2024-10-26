#include "pch.h"
#include "Map.h"
#include "MapObject.h"
#include "MapBuilder.h"

Map::Map() :
	m_Name{ "Untitled" },
	m_Objects{}
{
}

Map::Map(const json& j, TextureManager* textureManager) :
	m_Name{ j["Name"].get<std::string>() }
{
	if (j["Objects"].is_null()) {
		return;
	}
	std::vector<json> objects{ j["Objects"].get<std::vector<json>>() };
	for (int index{ 0 }; index < objects.size(); ++index) {
		m_Objects.push_back(new MapObject(objects[index], textureManager));
	}
}

Map::~Map()
{
	for (int index{ 0 }; index < m_Objects.size(); ++index) {
		delete m_Objects[index];
		m_Objects[index] = nullptr;
	}
	m_Objects.clear();
}

void Map::AddObject(MapObject* mapObject)
{
	m_Objects.push_back(mapObject);

	Sort();
}

void Map::RemoveObject(const MapObject* mapObject)
{
	std::vector<MapObject*> newObjects;

	for (int index{ 0 }; index < m_Objects.size(); ++index) {
		if (m_Objects[index] == mapObject) {
			m_Objects[index] = nullptr;
		}
		else {
			newObjects.push_back(m_Objects[index]);
		}
	}

	m_Objects = newObjects;
}

MapObject* Map::GetObjectOnPosition(const Point2f& position, std::vector<MapBuilderFilters> filter)
{
	for (int index{ int(m_Objects.size() - 1) }; index >= 0; --index) {
		MapBuilderFilters layerFilter{ DrawMiddleground };
		if (m_Objects[index]->GetIsBackground()) layerFilter = DrawBackground;
		if (m_Objects[index]->GetIsForeground()) layerFilter = DrawForeground;

		bool textureFilter{ m_Objects[index]->GetTextureIndex() != -1 && DoesFilterContain(filter,DrawTextures) };


		if (DoesFilterContain(filter, layerFilter))
		{
			bool isCollider{ m_Objects[index]->GetIsCollider() };
			if (!DoesFilterContain(filter, DrawColliders) && !isCollider)
			{
				continue;
			}

			if (m_Objects[index]->PointInBounds(position)) {
				return m_Objects[index];
			}
		}

	}
	return nullptr;
}

void Map::CheckOrder(const MapObject* changedObject)
{
	Sort();
}

std::vector<MapObject*> Map::GetObjects()
{
	return m_Objects;
}

std::string Map::GetName()
{
	return m_Name;
}

void Map::Draw(std::vector<MapBuilderFilters> filter)
{


	for (int index{ 0 }; index < m_Objects.size(); ++index) {
		MapBuilderFilters layerFilter{ DrawMiddleground };
		if (m_Objects[index]->GetIsBackground()) layerFilter = DrawBackground;
		if (m_Objects[index]->GetIsForeground()) layerFilter = DrawForeground;

		if (DoesFilterContain(filter, layerFilter))
		{
			m_Objects[index]->Draw(DoesFilterContain(filter, MapBuilderFilters::DrawTextures),
				DoesFilterContain(filter, MapBuilderFilters::DrawColliders));
		}
	}
}

void Map::ToJson(json& j)
{
	json arrayj;
	for (MapObject* object : m_Objects) {
		object->ToJson(arrayj);
	}

	j.push_back(json::object({
		{"Name",m_Name},
		{"Objects",arrayj},
		}));

}

void Map::SetName(const std::string& name)
{
	m_Name = name;
}

void Map::Sort()
{
	std::sort(m_Objects.begin(), m_Objects.end(), [](const MapObject* a, const MapObject* b)
		{
			if (a->GetIsBackground() || b->GetIsBackground())
			{
				if (a->GetIsBackground() && b->GetIsBackground()) {
					return a->m_Order < b->m_Order;
				}
				else
				{
					return a->GetIsBackground();
				}
			}

			if (a->GetIsForeground() || b->GetIsForeground())
			{
				if (a->GetIsForeground() && b->GetIsForeground()) {
					return a->m_Order < b->m_Order;
				}
				else
				{
					return !a->GetIsForeground();
				}
			}

			return a->m_Order < b->m_Order;
		});
}

int Map::CompareObjects(MapObject* first, MapObject* second)
{
	return first->m_Order > second->m_Order ? 1 : -1;
}

int Map::GetValidId()
{
	int id = 0;
	for (int index{ 0 }; index < m_Objects.size(); ++index)
	{
		if (id == m_Objects[index]->m_Index) {
			id++;
		}
		else {
			return id;
		}
	}
	return id;
}

bool Map::DoesFilterContain(std::vector<MapBuilderFilters> filter, MapBuilderFilters filtertype)
{
	for (int index{ 0 }; index < filter.size(); ++index)
	{
		if (filter[index] == filtertype) {
			return true;
		}
	}
	return false;
}
