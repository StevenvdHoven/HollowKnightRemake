#pragma once
#include <vector>
#include "json.hpp"
#include "utils.h"
#include "GUI.h"
#include "MapObject.h"

enum MapBuilderFilters
{
	DrawTextures,
	DrawColliders,
	DrawBackground,
	DrawMiddleground,
	DrawForeground,
};

class SnippingTool;
class Map;
class MapObject;
class MapObjectStatsUI;
class GUIText;
class GUIShape;
class Button;
class GUIinputTab;
class GUIToggle;
class TextureManager;
class GUIManager;
class MapBuilder final
{
public:
	MapBuilder(const Rectf& viewPort, TextureManager* textureManager, GUIManager* guiManager);
	~MapBuilder();

	Map* GetMap(const std::string& name) const;

	void Update(float elapsedTime);
	void Draw();
	void OnMouseClick(bool right);
	void OnMouseMotion(const Point2f& mousePos);
	void OnMouseClickUp();

	void OnNewObject(MapObject* object);

	void OnKeyEventUp(const SDL_KeyboardEvent& e);
	void OnKeyEventDown(const SDL_KeyboardEvent& e);
	void OnScroll(float direction);

	void SetHideUI(bool hide);
	Point2f GetConvertedMousePosition();

private:
	void CreateTopPanel();
	void CreateMapOptionPanel();
	void CreateButtons();
	void CreateToggles();
	void SetMap(int index);

	void UpdateMapOptions();

	void LoadMap();
	void ShrinkSave();
	void SaveMap();
	void DrawMapOptions();

	void DeleteMapObject();
	void CopyObject();
	void PasteObject();

	void AddFilter(MapBuilderFilters filter);
	void RemoveFilter(MapBuilderFilters filter);
	bool DoesFilterContain(MapBuilderFilters filtertype);

	const Rectf m_ViewPort;

	TextureManager* m_TextureManager;
	GUIManager* m_GUIManager;

	SnippingTool* m_SnippingTool;
	Map* m_CurrentMap;

	Point2f m_MousePos;
	Point2f m_DragOffset;
	bool m_Dragging;
	MapObject* m_CurrentSelectedObject;

	// Camera
	Point2f CameraPos;
	Vector2f CameraDirection;
	float m_CameraSpeed;
	float CameraScale;

	//Add Rotation & Scale & Copy/Paste
	float m_RotationDirection;
	float m_RotationSpeed;
	float m_ObjectRotation;
	float m_ObjectScaleDirection;
	Vector2f m_ObjectScale;
	Vector2f m_ObjectScaleNormal;
	MapObjectData m_CopiedData;

	//Filter
	std::vector<MapBuilderFilters> m_Filter;

	std::vector<Map*> m_AllMaps;

	GUI* m_Parent;

	// Top Panel GUI
	GUIShape* m_TopPanelShape;
	GUIinputTab* m_MapNameInput;

	//Map Options;
	GUIShape* m_MapSelectionBG;
	std::vector<Button*> m_MapOptionsButtons;

	// Top panel buttons & Input
	Button* m_CreateNewMapButton;
	Button* m_DeleteMapButton;
	Button* m_SwitchMapButton;
	Button* m_CreateColliderButton;

	// Filters
	GUIText* m_ShowTexturesText;
	GUIText* m_ShowCollidersText;
	GUIText* m_ShowBackgroundText;
	GUIText* m_ShowMiddlegroundText;
	GUIText* m_ShowForegroundText;

	GUIToggle* m_ShowTexturesToggle;
	GUIToggle* m_ShowCollidersToggle;
	GUIToggle* m_ShowBackgroundToggle;
	GUIToggle* m_ShowMiddlegroundToggle;
	GUIToggle* m_ShowForegroundToggle;

	// Map Object UI
	MapObjectStatsUI* m_StatsUI;

};

