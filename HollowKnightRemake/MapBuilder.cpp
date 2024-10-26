#include "pch.h"
#include "MapBuilder.h"
#include "SnippingTool.h"
#include "Map.h"
#include "Texture.h"
#include "MapObject.h"
#include <iostream>
#include <fstream>
#include "Transform.h"
#include "MapObjectStatsUI.h"
#include "GUIText.h"
#include "GUIinputTab.h"
#include "GUIShape.h"
#include "GUIText.h"
#include "GUIToggle.h"
#include "GUIManager.h"
#include "TextureManager.h"


MapBuilder::MapBuilder(const Rectf& viewPort, TextureManager* textureManager, GUIManager* guiManager) :
	m_ViewPort{ viewPort },
	m_TextureManager{ textureManager },
	m_GUIManager{ guiManager },
	m_SnippingTool{ nullptr },
	m_CurrentMap{ nullptr },
	m_MousePos{ 0,0 },
	m_DragOffset{ 0,0 },
	m_Dragging{ false },
	CameraPos{ Point2f{} },
	CameraDirection{ Vector2f{} },
	m_CameraSpeed{ 400 },
	CameraScale{ 1 },
	m_RotationDirection{ 0 },
	m_RotationSpeed{ 100 },
	m_ObjectRotation{ 0 },
	m_ObjectScaleDirection{ 0 },
	m_ObjectScale{},
	m_ObjectScaleNormal{},
	m_Filter{ DrawTextures, DrawColliders, DrawBackground, DrawMiddleground, DrawForeground},
	m_CopiedData{},
	m_CurrentSelectedObject{ nullptr },
	m_TopPanelShape{ nullptr },
	m_MapNameInput{ nullptr },
	m_CreateNewMapButton{ nullptr },
	m_DeleteMapButton{ nullptr },
	m_SwitchMapButton{ nullptr },
	m_StatsUI{ nullptr }
{
	m_Parent = new GUI();
	m_GUIManager->AddGUI(m_Parent);


	CreateTopPanel();

	CreateButtons();
	CreateToggles();

	LoadMap();
	CreateMapOptionPanel();

	m_StatsUI = new MapObjectStatsUI([this](const MapObject* obj) { m_CurrentMap->CheckOrder(obj); },m_GUIManager);
	m_SnippingTool = new SnippingTool(viewPort, m_TextureManager,m_GUIManager, [this](MapObject* obj) { OnNewObject(obj); });
}

MapBuilder::~MapBuilder()
{
	for (int index{ 0 }; index < m_AllMaps.size(); ++index) 
	{
		delete m_AllMaps[index];
	}
	m_AllMaps.clear();

	delete m_SnippingTool;
	delete m_StatsUI;

	m_Parent = nullptr;
	m_TopPanelShape = nullptr;
	m_MapNameInput = nullptr;
	m_MapSelectionBG = nullptr;

	m_MapOptionsButtons.clear();

	m_CreateNewMapButton = nullptr;
	m_DeleteMapButton = nullptr;
	m_SwitchMapButton = nullptr;
	m_CreateColliderButton = nullptr;

	m_ShowTexturesText = nullptr;
	m_ShowCollidersText = nullptr;
	m_ShowBackgroundText = nullptr;
	m_ShowMiddlegroundText = nullptr;
	m_ShowForegroundText = nullptr;

	m_ShowTexturesToggle = nullptr;
	m_ShowCollidersToggle = nullptr;
	m_ShowBackgroundToggle = nullptr;
	m_ShowMiddlegroundToggle = nullptr;
	m_ShowForegroundToggle = nullptr;
	
}

Map* MapBuilder::GetMap(const std::string& name) const
{
	for (int index{ 0 }; index < m_AllMaps.size(); ++index) {
		if (m_AllMaps[index]->GetName() == name) {
			return m_AllMaps[index];
		}
	}
	return nullptr;
}

void MapBuilder::Update(float elapsedTime)
{
	if (!GUIinputTab::m_Typing) {
		CameraPos += CameraDirection * m_CameraSpeed * elapsedTime;
	}
	m_ObjectRotation += m_RotationDirection * m_RotationSpeed * elapsedTime;
	m_ObjectScale += m_ObjectScaleNormal * m_ObjectScaleDirection * elapsedTime;

	if (m_Dragging) {
		if (m_CurrentSelectedObject != nullptr) {
			m_CurrentSelectedObject->SetRotation(m_ObjectRotation);
			m_CurrentSelectedObject->SetScale(m_ObjectScale);
		}
	}
	m_StatsUI->Update();
}

void MapBuilder::Draw()
{
	Transform trans;
	trans.Position = Vector2f(CameraPos.x,CameraPos.y);
	trans.Scale = Vector2f(CameraScale, CameraScale);
	trans.ApplyTransformation();
	m_CurrentMap->Draw(m_Filter);
	trans.ResetTransformation();

	m_SnippingTool->Draw();
}

void MapBuilder::OnMouseClick(bool right)
{
	m_SnippingTool->OnClick(m_MousePos);
	
	if (m_GUIManager->IsMouseOnGui() || m_SnippingTool->IsMouseOverSnippingTool(m_MousePos)) {
		return;
	}
	MapObject* clickedObject{ m_CurrentMap->GetObjectOnPosition(GetConvertedMousePosition(),m_Filter) };

	if (clickedObject != nullptr) {

		m_CurrentSelectedObject = clickedObject;
		m_Dragging = true;
		
		m_DragOffset = (Point2f)(clickedObject->GetPosition() - GetConvertedMousePosition());
		m_ObjectRotation = m_CurrentSelectedObject->GetRotation();
		m_ObjectScale = m_CurrentSelectedObject->GetScale();
		m_ObjectScaleNormal = m_ObjectScale / m_ObjectScale.Length();
	}
	else if (m_CurrentSelectedObject != nullptr) {
		m_CurrentSelectedObject = nullptr;
	}
}

void MapBuilder::OnMouseMotion(const Point2f& mousePos)
{
	float xScaler = 1920.f / 1280.f;
	float yScaler = 1080.f / 800.f;

	m_MousePos = mousePos;
	m_MousePos.x = m_MousePos.x * xScaler;
	m_MousePos.y = m_MousePos.y * yScaler;

	m_SnippingTool->OnMouseMotion(m_MousePos);

	if (m_Dragging) {
		if (m_CurrentSelectedObject != nullptr) {
			
			m_CurrentSelectedObject->SetPosition(GetConvertedMousePosition() + (Vector2f)m_DragOffset);
			
		}
	}
}

void MapBuilder::OnMouseClickUp()
{
	m_SnippingTool->OnClickUp(m_MousePos);

	if (m_Dragging) {
		m_Dragging = false;
		m_ObjectRotation = 0;

		if (m_GUIManager->IsMouseOnGui() || m_SnippingTool->IsMouseOverSnippingTool(m_MousePos)) {
			return;
		}

		MapObject* object{ m_CurrentMap->GetObjectOnPosition(GetConvertedMousePosition(),m_Filter) };
		if (object != nullptr) {
			m_StatsUI->SetObject(object);
			m_StatsUI->SetHide(false);
		}
	}

	
}

void MapBuilder::OnNewObject(MapObject* object)
{
	m_CurrentMap->AddObject(object);
	object->SetPosition(Point2f(m_ViewPort.width * 0.5f, m_ViewPort.height * 0.5f) - Vector2f(CameraPos));
}

void MapBuilder::OnKeyEventUp(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_SPACE) {
		SaveMap();
	}

	if (e.keysym.sym == SDLK_DELETE) {
		DeleteMapObject();
	}

	if (e.keysym.sym == SDLK_c && !GUIinputTab::m_Typing) {
		CopyObject();
	}

	if (e.keysym.sym == SDLK_v && !GUIinputTab::m_Typing) {
		PasteObject();
	}

	if (e.keysym.sym == SDLK_UP) {
		m_ObjectScaleDirection = 0;
	}
	if (e.keysym.sym == SDLK_DOWN) {
		m_ObjectScaleDirection = 0;
	}

	if (e.keysym.sym == SDLK_w) {
		CameraDirection.y = 0;
	}
	if (e.keysym.sym == SDLK_s) {
		CameraDirection.y = 0;
	}
	if (e.keysym.sym == SDLK_a) {
		CameraDirection.x = 0;
	}
	if (e.keysym.sym == SDLK_d) {
		CameraDirection.x = 0;
	}

	if (e.keysym.sym == SDLK_LEFT) {
		m_RotationDirection = 0;
	}
	else if (e.keysym.sym == SDLK_RIGHT) {
		m_RotationDirection = 0;
	}
}

void MapBuilder::OnKeyEventDown(const SDL_KeyboardEvent& e)
{
	

	if (e.keysym.sym == SDLK_w) {
		CameraDirection.y = -1;
	}
	if (e.keysym.sym == SDLK_s) {
		CameraDirection.y = 1;
	}
	if (e.keysym.sym == SDLK_a) {
		CameraDirection.x = 1;
	}
	if (e.keysym.sym == SDLK_d) {
		CameraDirection.x = -1;
	}

	if (e.keysym.sym == SDLK_UP) {
		m_ObjectScaleDirection = 1;
	}
	if (e.keysym.sym == SDLK_DOWN) {
		m_ObjectScaleDirection = -1;
	}

	if (e.keysym.sym == SDLK_LEFT) {
		m_RotationDirection = 1;
	}
	else if (e.keysym.sym == SDLK_RIGHT) {
		m_RotationDirection = -1;
	}
}

void MapBuilder::OnScroll(float direction)
{
	float increment{ direction > 0.f ? 0.1f : -0.1f };
	CameraScale += increment;

	std::cout << "Scale: " << CameraScale << std::endl;
}

void MapBuilder::SetHideUI(bool hide)
{
	m_Parent->SetHiding(hide);
	m_Parent->SetInteractable(!hide);
	m_StatsUI->SetHide(true);
}

Point2f MapBuilder::GetConvertedMousePosition()
{
	Transform trans;
	trans.Position = -Vector2f(CameraPos);
	Point2f transPoint{ utils::ApplyMatrix(trans,m_MousePos) };
	trans.Position = Vector2f{};
	trans.Scale = Vector2f(1/CameraScale, 1/CameraScale);

	return utils::ApplyMatrix(trans,transPoint);
}

void MapBuilder::CreateTopPanel()
{
	const float halfWidth{ 1920 * 0.5f };
	const float halfHeight{ 50 };
	std::vector<Point2f> vertices{
		Point2f{-halfWidth,-halfHeight},
		Point2f{-halfWidth,halfHeight},
		Point2f{halfWidth,halfHeight},
		Point2f{halfWidth, -halfHeight},
	};

	m_TopPanelShape = new GUIShape(Point2f{ halfWidth,1080 - halfHeight }, vertices, white);
	m_TopPanelShape->SetParent(m_Parent);

	std::function<void(const std::string&)> onValue{ [this](const std::string& value)
		{
			m_CurrentMap->SetName(value);
			UpdateMapOptions();
		}
	};
	m_MapNameInput = new GUIinputTab(
		Rectf(330, 1045, 150, 25),
		m_GUIManager->GetFont(arial_14),
		black,
		white,
		onValue);
	m_MapNameInput->SetValue("Untitled");

	m_MapNameInput->SetParent(m_Parent);
}

void MapBuilder::CreateMapOptionPanel()
{
	// GUI background
	m_MapSelectionBG = new GUIShape(Rectf{ 10, 10, 1700, 500 }, white);
	m_MapSelectionBG->SetParent(m_Parent);

	// Create buttons
	UpdateMapOptions();

}

void MapBuilder::CreateButtons()
{
	// New map
	std::function<void()> newMapCallback{ [this]() 
		{
			Map* newMap{new Map()};
			m_AllMaps.push_back(newMap);
			m_CurrentMap = newMap;
			m_MapNameInput->SetValue(m_CurrentMap->GetName());
			m_StatsUI->SetHide(true);

			UpdateMapOptions();
		}
	};

	m_CreateNewMapButton = new Button(
		Rectf{ 10,1020,150,50 },
		black,
		white,
		"New map",
		m_GUIManager->GetFont(arial_14),
		ButtonTypes::Up,
		newMapCallback);

	m_CreateNewMapButton->SetParent(m_Parent);

	// Delete map
	std::function<void()> deleteMapCallback{ [this]()
		{
			m_StatsUI->SetHide(true); 
			for (int index{0}; index < m_AllMaps.size(); ++index) 
			{
				if (m_AllMaps[index] == m_CurrentMap) 
				{
					m_AllMaps[index] = nullptr;
					delete m_CurrentMap;

					for (int second{ 0 }; second < m_AllMaps.size(); ++second) 
					{
						if (m_AllMaps[index] != nullptr) {
							m_CurrentMap = m_AllMaps[index];
							m_MapNameInput->SetValue(m_CurrentMap->GetName());
							UpdateMapOptions();
							return;
						}
					}
					
					m_AllMaps.clear();
					m_CurrentMap = new Map();
					m_AllMaps.push_back(m_CurrentMap);
					m_MapNameInput->SetValue(m_CurrentMap->GetName());
					UpdateMapOptions();
				}
			}
		}
	};

	m_DeleteMapButton = new Button(
		Rectf{ 170,1020,150,50 },
		black,
		white,
		"Delete map",
		m_GUIManager->GetFont(arial_14),
		ButtonTypes::Up,
		deleteMapCallback);

	m_DeleteMapButton->SetParent(m_Parent);

	// Switch Map button
	std::function<void()> switchmap{ [this]()
		{
			m_StatsUI->SetHide(true);
			if(m_MapSelectionBG->IsHidden()){
				m_MapSelectionBG->SetHiding(false);
				m_MapSelectionBG->SetInteractable(true);
			}
			else {
				m_MapSelectionBG->SetHiding(true);
				m_MapSelectionBG->SetInteractable(false);
			}
		}
	};

	m_SwitchMapButton = new Button(
		Rectf(330, 1020, 150, 25),
		black,
		white,
		"Switch map",
		m_GUIManager->GetFont(arial_14),
		ButtonTypes::Up,
		switchmap);

	m_SwitchMapButton->SetParent(m_Parent);

	std::function<void()> createCollider{ [this]()
		{
			MapObject* mapObject{new MapObject(0,-1,Point2f(m_ViewPort.width * 0.5f, m_ViewPort.height * 0.5f) - Vector2f(CameraPos),{},m_TextureManager)};
			mapObject->SetBounds(Rectf{ 0,0,100,100 });
			m_CurrentMap->AddObject(mapObject);
		}
	};

	m_CreateColliderButton = new Button(
		Rectf(490, 1020, 150, 50),
		black,
		white,
		"Collider",
		m_GUIManager->GetFont(arial_14),
		ButtonTypes::Up,
		createCollider);
	m_CreateColliderButton->SetParent(m_Parent);
}

void MapBuilder::CreateToggles()
{
	std::function<void(bool)> onShowTexturesToggle{ [this](bool value)
		{
			if (value) {
				AddFilter(DrawTextures);
			}
			else {
				RemoveFilter(DrawTextures);
			}
			
		}
	};

	m_ShowTexturesText = new GUIText(Point2f{ 770, 1020 }, "Show Textures", m_GUIManager->GetFont(arial_14), black, TextAlign::Left);
	m_ShowTexturesToggle = new GUIToggle(
		Rectf{ 720, 1020, 50, 50 },
		black,
		white,
		onShowTexturesToggle);
	m_ShowTexturesText->SetParent(m_Parent);
	m_ShowTexturesToggle->SetParent(m_Parent);
	m_ShowTexturesToggle->SetValue(true);

	std::function<void(bool)> onShowCollidersToggle{ [this](bool value)
		{
			if (value) {
				RemoveFilter(DrawColliders);
			}
			else {
				AddFilter(DrawColliders);
			}

		}
	};

	m_ShowCollidersText = new GUIText(Point2f{ 970, 1020 }, "Only Colliders", m_GUIManager->GetFont(arial_14), black, TextAlign::Left);
	m_ShowCollidersToggle = new GUIToggle(
		Rectf{ 920, 1020, 50, 50 },
		black,
		white,
		onShowCollidersToggle);
	m_ShowCollidersText->SetParent(m_Parent);
	m_ShowCollidersToggle->SetParent(m_Parent); 

	std::function<void(bool)> onShowBackgroundToggle{ [this](bool value)
		{
			if (value) {
				AddFilter(DrawBackground);
			}
			else {
				if (!DoesFilterContain(DrawMiddleground) && !DoesFilterContain(DrawForeground) && !value) {
				m_ShowBackgroundToggle->SetValue(true);
				return;
			}
				RemoveFilter(DrawBackground);
			}

		}
	};

	m_ShowBackgroundText = new GUIText(Point2f{ 1170, 1020 }, "Show background", m_GUIManager->GetFont(arial_14), black, TextAlign::Left);
	m_ShowBackgroundToggle = new GUIToggle(
		Rectf{ 1120, 1020, 50, 50 },
		black,
		white,
		onShowBackgroundToggle);
	m_ShowBackgroundText->SetParent(m_Parent);
	m_ShowBackgroundToggle->SetParent(m_Parent);
	m_ShowBackgroundToggle->SetValue(true);

	std::function<void(bool)> onShowMiddleToggle{ [this](bool value)
		{
			if (value) {
				AddFilter(DrawMiddleground);
			}
			else {
				if (!DoesFilterContain(DrawBackground) && !DoesFilterContain(DrawForeground) && !value) {
				m_ShowMiddlegroundToggle->SetValue(true);
				return;
			}
				RemoveFilter(DrawMiddleground);
			}

		}
	};

	m_ShowMiddlegroundText = new GUIText(Point2f{ 1370, 1020 }, "Show middleground", m_GUIManager->GetFont(arial_14), black, TextAlign::Left);
	m_ShowMiddlegroundToggle = new GUIToggle(
		Rectf{ 1320, 1020, 50, 50 },
		black,
		white,
		onShowMiddleToggle);
	m_ShowMiddlegroundText->SetParent(m_Parent);
	m_ShowMiddlegroundToggle->SetParent(m_Parent);
	m_ShowMiddlegroundToggle->SetValue(true);

	std::function<void(bool)> onShowForegroundToggle{ [this](bool value)
		{
			if (value) {
				AddFilter(DrawForeground);
			}
			else {

				if (!DoesFilterContain(DrawMiddleground) && !DoesFilterContain(DrawBackground) && !value) {
					m_ShowForegroundToggle->SetValue(true);
					return;
				}
				RemoveFilter(DrawForeground);
			}

		}
	};

	m_ShowForegroundText = new GUIText(Point2f{ 1570, 1020 }, "Show foreground", m_GUIManager->GetFont(arial_14), black, TextAlign::Left);
	m_ShowForegroundToggle = new GUIToggle(
		Rectf{ 1520, 1020, 50, 50 },
		black,
		white,
		onShowForegroundToggle);
	m_ShowForegroundText->SetParent(m_Parent);
	m_ShowForegroundToggle->SetParent(m_Parent);
	m_ShowForegroundToggle->SetValue(true);
}

void MapBuilder::SetMap(int index)
{
	m_CurrentMap = m_AllMaps[index];
	m_MapNameInput->SetValue(m_CurrentMap->GetName());
	m_StatsUI->SetHide(true);
}

void MapBuilder::UpdateMapOptions()
{
	m_MapSelectionBG->SetParent(m_Parent);

	std::vector<GUI*> childeren{ m_MapSelectionBG->GetChilderen() };
	for (GUI* child : childeren) {
		m_MapSelectionBG->RemoveChild(child);
		delete child;
	}
	childeren.clear();

	const float buttonWidth{ 100 };
	const float buttonHeight{ 40 };

	const int maxRows{ 10 };

	int currentRow{ 0 };
	int currentCollum{ 0 };
	const Point2f topLeft{ 20,490 };

	for (int index{ 0 }; index < m_AllMaps.size(); ++index) {
		if (m_AllMaps[index] == nullptr) {
			continue;
		}

		if (currentRow > maxRows) {
			currentRow = 0;
			++currentCollum;
		}

		Point2f pos{ topLeft + Vector2f(currentCollum * buttonWidth, currentRow * -buttonHeight) };

		std::function<void()> callback{ [this,index]()
			{
				SetMap(index);
				m_MapSelectionBG->SetHiding(true);
				m_MapSelectionBG->SetInteractable(false);
			}
		};

		Button* button{ new Button(
			Rectf{pos.x,pos.y - buttonHeight,buttonWidth,buttonHeight},
			black,
			white,
			m_AllMaps[index]->GetName(),
			m_GUIManager->GetFont(arial_14),
			ButtonTypes::Up,
			callback) };

		button->SetParent(m_MapSelectionBG);
		++currentRow;
		
	}
	
	m_MapSelectionBG->SetHiding(true);
}

void MapBuilder::LoadMap()
{
	std::fstream stream( "Maps.json" );
	if (stream.is_open() && stream.good()) {
		json j{ json::parse(stream) };

		if (j.is_object()) {

			std::vector<json> maps = j["Maps"].get<std::vector<json>>();
			for (int index{ 0 }; index < maps.size(); ++index) {
				m_AllMaps.push_back(new Map(maps[index],m_TextureManager));
			}
			m_CurrentMap = m_AllMaps[0];
		}
		
	}
	else {
		m_CurrentMap = new Map();
	}
	m_MapNameInput->SetValue(m_CurrentMap->GetName());
}

void MapBuilder::ShrinkSave()
{
	std::vector<Map*> shrinkedMap;
	for (Map* map : m_AllMaps) {
		if (map != nullptr) {
			shrinkedMap.push_back(map);
		}
	}
	m_AllMaps = shrinkedMap;
}

void MapBuilder::SaveMap()
{
	ShrinkSave();

	json j;
	json map;
	for (int index{ 0 }; index < m_AllMaps.size(); ++index) {
		m_AllMaps[index]->ToJson(map);
	}

	j["Maps"] = map;

	

	std::ofstream stream("Maps.json");
	stream << j.dump(4);
	stream.close();

	std::cout << "Map Saved";
}

void MapBuilder::DrawMapOptions()
{
	const float optionSpacing{ 5 };
	float optionYPosition{ 30 };

	utils::SetColor(Color4f(0, 0, 0, 1));
	utils::FillRect(0, 0, 200, 700);

	for (int row{ 0 }; row < m_AllMaps.size(); ++row) 
	{
		utils::SetColor(Color4f{ 100,100,100,1 });
		utils::FillRect(10, optionYPosition, 100, 50);
		
	}
}

void MapBuilder::DeleteMapObject()
{
	MapObject* currentObject{ m_CurrentMap->GetObjectOnPosition(GetConvertedMousePosition(),m_Filter) };
	if (currentObject != nullptr) 
	{
		m_CurrentMap->RemoveObject(currentObject);
		delete currentObject;
	}
}

void MapBuilder::CopyObject()
{
	MapObject* currentObject{ m_CurrentMap->GetObjectOnPosition(GetConvertedMousePosition(),m_Filter)};
	if (currentObject != nullptr) {
		m_CopiedData = currentObject->GetData();
		std::cout << "Object Copied" << std::endl;
	}
	
}

void MapBuilder::PasteObject()
{
	m_CopiedData.Position = GetConvertedMousePosition();
	MapObject* currentObject{ new MapObject(m_CopiedData,m_TextureManager) };
	m_CurrentMap->AddObject(currentObject);
	std::cout << "Pasted Object" << std::endl;
}

void MapBuilder::AddFilter(MapBuilderFilters filter)
{
	if (!DoesFilterContain(filter)) 
	{
		m_Filter.push_back(filter);
	}
}

void MapBuilder::RemoveFilter(MapBuilderFilters filter)
{
	for (int index{ 0 }; index < m_Filter.size(); ++index)
	{
		if (m_Filter[index] == filter) {
			m_Filter.erase(m_Filter.begin() + index);
			return;
		}
	}
}

bool MapBuilder::DoesFilterContain(MapBuilderFilters filtertype)
{
	for (int index{ 0 }; index < m_Filter.size(); ++index)
	{
		if (m_Filter[index] == filtertype) {
			return true;
		}
	}
	return false;
}


