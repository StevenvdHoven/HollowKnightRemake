#pragma once
#include <functional>

class MapObject;
class GUIShape;
class Button;
class GUIText;
class GUIinputTab;
class GUIToggle;
class GUIManager;
class MapObjectStatsUI final
{
public:
	MapObjectStatsUI(std::function<void(const MapObject*)> callback, GUIManager* guiManager);
	~MapObjectStatsUI();
	void CreateUI();
	void CreateInputTabs();
	void CreateToggles();
	void Update();

	void SetObject(MapObject* object);
	void SetHide(bool hide);
	void SetActive(bool active);

private:
	GUIManager* m_GUIManager;

	MapObject* m_CurrentObject;

	GUIShape* m_Panel;
	
	GUIText* m_PositionTextX;
	GUIText* m_PositionTextY;
	GUIText* m_RotationText;
	GUIText* m_ScaleXText;
	GUIText* m_ScaleYText;
	GUIText* m_OrderText;
	GUIText* m_TagText;
	GUIText* m_IsCollisionText;
	GUIText* m_IsTriggerText;
	GUIText* m_IsBackgroundText;
	GUIText* m_IsForegroundText;
	GUIText* m_WidthText;
	GUIText* m_HeightText;

	GUIText* m_TintRedText;
	GUIText* m_TintGreenText;
	GUIText* m_TintBlueText;
	GUIText* m_TintAlphaText;

	GUIinputTab* m_PositionInputX;
	GUIinputTab* m_PositionInputY;
	GUIinputTab* m_RotationInput;
	GUIinputTab* m_ScaleXInput;
	GUIinputTab* m_ScaleYInput;
	GUIinputTab* m_OrderInput;
	GUIinputTab* m_TagInput;

	GUIinputTab* m_WidthInput;
	GUIinputTab* m_HeightInput;

	GUIinputTab* m_TintRedInput;
	GUIinputTab* m_TintGreenInput;
	GUIinputTab* m_TintBlueInput;
	GUIinputTab* m_TintAlphaInput;

	GUIToggle* m_IsCollisionToggle;
	GUIToggle* m_IsTriggerToggle;
	GUIToggle* m_IsBackgroundToggle;
	GUIToggle* m_IsForegroundToggle;

	std::function<void(const MapObject*)> m_OnOrderChanged;

};

