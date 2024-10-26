#include "pch.h"
#include "MapObjectStatsUI.h"
#include "GUIShape.h"
#include "GUIText.h"
#include "Button.h"
#include "GUIinputTab.h"
#include "GUIToggle.h"
#include "MapObject.h"
#include "GUIManager.h"

MapObjectStatsUI::MapObjectStatsUI(std::function<void(const MapObject*)> callback, GUIManager* guiManager) :
	m_GUIManager{ guiManager},
	m_CurrentObject{ nullptr },
	m_Panel{ nullptr },
	m_PositionTextX{ nullptr},
	m_PositionTextY{nullptr},
	m_RotationText{ nullptr },
	m_ScaleXText{ nullptr },
	m_ScaleYText{nullptr},
	m_OrderText{ nullptr },
	m_OnOrderChanged{ callback }
{
	CreateUI();
}

MapObjectStatsUI::~MapObjectStatsUI()
{

}

void MapObjectStatsUI::CreateUI()
{
	m_Panel = new GUIShape(Rectf(10, 10, 200, 400),Color4f(.1f,.1f,.1f,1.f),true);
	m_GUIManager->AddGUI(m_Panel);

	TTF_Font* commonFont{ m_GUIManager->GetFont(arial_14) };
	m_PositionTextX = new GUIText(Point2f(20, 400), "X:", commonFont, white);
	m_PositionTextY = new GUIText(Point2f(120, 400), "Y:", commonFont, white);
	m_RotationText = new GUIText(Point2f(105, 370), "Rotation:", commonFont, white);
	m_ScaleXText = new GUIText(Point2f(55, 330), "Scale X:", commonFont, white);
	m_ScaleYText = new GUIText(Point2f(155, 330), "Scale Y:", commonFont, white);
	m_OrderText = new GUIText(Point2f(105, 290), "Order:", commonFont, white);
	m_TagText = new GUIText(Point2f(105, 250), "Tag:", commonFont, white);
	m_IsCollisionText = new GUIText(Point2f(40, 190), "IsCollider", commonFont, white,TextAlign::Left);
	m_IsTriggerText = new GUIText(Point2f(135, 190), "IsTrigger", commonFont, white, TextAlign::Left);
	m_IsBackgroundText = new GUIText(Point2f(80, 160), "IsBackground", commonFont, white, TextAlign::Left);
	m_IsForegroundText = new GUIText(Point2f(80, 130), "IsForeground", commonFont, white, TextAlign::Left);
	m_WidthText = new GUIText(Point2f(55, 100), "Width:", commonFont, white);
	m_HeightText = new GUIText(Point2f(155, 100), "Height:", commonFont, white);

	m_TintRedText = new GUIText(Point2f(20, 40), "R:", commonFont, white);
	m_TintGreenText = new GUIText(Point2f(60, 40), "G:", commonFont, white);
	m_TintBlueText = new GUIText(Point2f(100, 40), "B:", commonFont, white);
	m_TintAlphaText = new GUIText(Point2f(140, 40), "A:", commonFont, white);


	m_PositionTextX->SetParent(m_Panel);
	m_PositionTextY->SetParent(m_Panel);

	m_RotationText->SetParent(m_Panel);

	m_ScaleXText->SetParent(m_Panel);
	m_ScaleYText->SetParent(m_Panel);

	m_OrderText->SetParent(m_Panel);
	m_TagText->SetParent(m_Panel);

	m_IsCollisionText->SetParent(m_Panel);
	m_IsTriggerText->SetParent(m_Panel);

	m_IsBackgroundText->SetParent(m_Panel);
	m_IsForegroundText->SetParent(m_Panel);

	m_WidthText->SetParent(m_Panel);
	m_HeightText->SetParent(m_Panel);

	m_TintRedText->SetParent(m_Panel);
	m_TintBlueText->SetParent(m_Panel);
	m_TintGreenText->SetParent(m_Panel);
	m_TintAlphaText->SetParent(m_Panel);

	CreateInputTabs();
	CreateToggles();

	SetHide(true);
}

void MapObjectStatsUI::CreateInputTabs()
{
	std::function<void(const std::string& value)> onPositionXChange{ [this](const std::string& value)
		{
			Point2f pos{m_CurrentObject->GetPosition()};
			pos.x = std::stof(value);
			m_CurrentObject->SetPosition(pos);
		}
	};
	m_PositionInputX = new GUIinputTab(Rectf(30, 390, 80, 20), m_GUIManager->GetFont(arial_14), black, white, onPositionXChange);
	m_PositionInputX->SetParent(m_Panel);

	std::function<void(const std::string& value)> onPositionYChange{ [this](const std::string& value)
		{
			Point2f pos{m_CurrentObject->GetPosition()};
			pos.y = std::stof(value);
			m_CurrentObject->SetPosition(pos);
		}
	};
	m_PositionInputY = new GUIinputTab(Rectf(130, 390, 80, 20), m_GUIManager->GetFont(arial_14), black, white, onPositionYChange);
	m_PositionInputY->SetParent(m_Panel);

	std::function<void(const std::string& value)> onRotationChange{ [this](const std::string& value)
		{
			m_CurrentObject->SetRotation(std::stof(value));
		}
	};
	m_RotationInput = new GUIinputTab(Rectf(55, 340, 100, 20), m_GUIManager->GetFont(arial_14), black, white, onRotationChange);
	m_RotationInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onScaleXChange{ [this](const std::string& value)
		{
			Vector2f scale{m_CurrentObject->GetScale()};
			scale.x = std::stof(value);
			m_CurrentObject->SetScale(scale);
		}
	};
	m_ScaleXInput = new GUIinputTab(Rectf(15, 300, 80, 20), m_GUIManager->GetFont(arial_14), black, white, onScaleXChange);
	m_ScaleXInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onScaleYChange{ [this](const std::string& value)
		{
			Vector2f scale{m_CurrentObject->GetScale()};
			scale.y = std::stof(value);
			m_CurrentObject->SetScale(scale);
		}
	};
	m_ScaleYInput = new GUIinputTab(Rectf(115, 300, 80, 20), m_GUIManager->GetFont(arial_14), black, white, onScaleYChange);
	m_ScaleYInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onOrderChange{ [this](const std::string& value)
		{
			m_CurrentObject->m_Order = std::stoi(value);
			m_OnOrderChanged(m_CurrentObject);
		}
	};
	m_OrderInput = new GUIinputTab(Rectf(55, 260, 100, 20), m_GUIManager->GetFont(arial_14), black, white, onOrderChange);
	m_OrderInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onTagChange{ [this](const std::string& value)
		{
			m_CurrentObject->SetTag(value);
			m_TagInput->SetValue(m_CurrentObject->GetTag());
		}
	};
	m_TagInput = new GUIinputTab(Rectf(55, 220, 100, 20), m_GUIManager->GetFont(arial_14), black, white, onTagChange);
	m_TagInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onWidthChange{ [this](const std::string& value)
		{
			Rectf bounds{m_CurrentObject->GetBounds()};
			bounds.width = std::stoi(value);
			m_CurrentObject->SetBounds(bounds);
		}
	};
	m_WidthInput = new GUIinputTab(Rectf(15, 70, 80, 20), m_GUIManager->GetFont(arial_14), black, white, onWidthChange);
	m_WidthInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onHeightChange{ [this](const std::string& value)
		{
			Rectf bounds{m_CurrentObject->GetBounds()};
			bounds.height = std::stoi(value);
			m_CurrentObject->SetBounds(bounds);
		}
	};
	m_HeightInput = new GUIinputTab(Rectf(115, 70, 80, 20), m_GUIManager->GetFont(arial_14), black, white, onHeightChange);
	m_HeightInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onTintRedChange{ [this](const std::string& value)
		{
			Color4f tint{m_CurrentObject->GetTint()};
			tint.r = std::stof(value);
			m_CurrentObject->SetTint(tint);
		}
	};
	m_TintRedInput = new GUIinputTab(Rectf(30, 30, 20, 20), m_GUIManager->GetFont(arial_14), black, white, onTintRedChange);
	m_TintRedInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onTintGreenChange{ [this](const std::string& value)
		{
			Color4f tint{m_CurrentObject->GetTint()};
			tint.g = std::stof(value);
			m_CurrentObject->SetTint(tint);
		}
	};
	m_TintGreenInput = new GUIinputTab(Rectf(70, 30, 20, 20), m_GUIManager->GetFont(arial_14), black, white, onTintGreenChange);
	m_TintGreenInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onTintBlueChange{ [this](const std::string& value)
		{
			Color4f tint{m_CurrentObject->GetTint()};
			tint.b = std::stof(value);
			m_CurrentObject->SetTint(tint);
		}
	};
	m_TintBlueInput = new GUIinputTab(Rectf(110, 30, 20, 20), m_GUIManager->GetFont(arial_14), black, white, onTintBlueChange);
	m_TintBlueInput->SetParent(m_Panel);

	std::function<void(const std::string& value)> onTintAlphaChange{ [this](const std::string& value)
		{
			Color4f tint{m_CurrentObject->GetTint()};
			tint.a = std::stof(value);
			m_CurrentObject->SetTint(tint);
		}
	};
	m_TintAlphaInput = new GUIinputTab(Rectf(150, 30, 20, 20), m_GUIManager->GetFont(arial_14), black, white, onTintAlphaChange);
	m_TintAlphaInput->SetParent(m_Panel);

}

void MapObjectStatsUI::CreateToggles()
{
	std::function<void(bool)> onCollisionValue
	{ 
		[this](bool value)
		{
			m_CurrentObject->SetIsCollider(value);
		}
	};
	m_IsCollisionToggle = new GUIToggle(Rectf(15, 190, 20, 20),black,white,onCollisionValue);
	m_IsCollisionToggle->SetParent(m_Panel);

	std::function<void(bool)> onTriggerValue
	{
		[this](bool value)
		{
			m_CurrentObject->SetIsTrigger(value);
		}
	};
	m_IsTriggerToggle = new GUIToggle(Rectf(110, 190, 20, 20), black, white, onTriggerValue);
	m_IsTriggerToggle->SetParent(m_Panel);

	std::function<void(bool)> onIsBackgroundValue{ [this](bool value)
		{
			m_CurrentObject->SetIsBackground(value);
			m_OnOrderChanged(m_CurrentObject);
		}
	};
	m_IsBackgroundToggle = new GUIToggle(Rectf(55, 160, 20, 20), black, white, onIsBackgroundValue);
	m_IsBackgroundToggle->SetParent(m_Panel);

	std::function<void(bool)> onIsForegroundValue{ [this](bool value)
		{
			m_CurrentObject->SetIsForeground(value);
			m_OnOrderChanged(m_CurrentObject);
		}
	};
	m_IsForegroundToggle = new GUIToggle(Rectf(55, 130, 20, 20), black, white, onIsForegroundValue);
	m_IsForegroundToggle->SetParent(m_Panel);
}

void MapObjectStatsUI::Update()
{
	if (m_CurrentObject == nullptr) {
		return;
	}

	const Point2f& pos{ m_CurrentObject->GetPosition() };
	if (!m_PositionInputX->IsTyping()) 
	{
		m_PositionInputX->SetValue(std::to_string(pos.x));
	}
	if (!m_PositionInputY->IsTyping())
	{
		m_PositionInputY->SetValue(std::to_string(pos.y));
	}
	
	if (!m_RotationInput->IsTyping())
	{
		const float rot{ m_CurrentObject->GetRotation() };
		m_RotationInput->SetValue(std::to_string(rot));
	}

	if (!m_ScaleXInput->IsTyping())
	{
		const float scale{ m_CurrentObject->GetScale().x };
		m_ScaleXInput->SetValue(std::to_string(scale));
	}

	if (!m_ScaleYInput->IsTyping()) 
	{
		const float scale{ m_CurrentObject->GetScale().y };
		m_ScaleYInput->SetValue(std::to_string(scale));
	}

	if (!m_OrderInput->IsTyping())
	{
		const int order{ m_CurrentObject->m_Order };
		m_OrderInput->SetValue(std::to_string(order));
	}
}

void MapObjectStatsUI::SetObject(MapObject* object)
{
	m_CurrentObject = object;

	m_TagInput->SetValue(object->GetTag());
	m_IsCollisionToggle->SetValue(object->GetIsCollider());
	m_IsTriggerToggle->SetValue(object->GetIsTrigger());
	m_IsBackgroundToggle->SetValue(object->GetIsBackground());
	m_IsForegroundToggle->SetValue(object->GetIsForeground());
	
	Rectf bound{ object->GetBounds() };
	m_WidthInput->SetValue(std::to_string( bound.width));
	m_HeightInput->SetValue(std::to_string(bound.height));

	const Color4f tint{ object->GetTint() };
	m_TintRedInput->SetValue(std::to_string(tint.r),3);
	m_TintGreenInput->SetValue(std::to_string(tint.g),3);
	m_TintBlueInput->SetValue(std::to_string(tint.b),3);
	m_TintAlphaInput->SetValue(std::to_string(tint.a),3);
}

void MapObjectStatsUI::SetHide(bool hide)
{
	m_Panel->SetHiding(hide);
	m_Panel->SetInteractable(!hide);
}

void MapObjectStatsUI::SetActive(bool active)
{
	SetHide(!active);

	m_Panel->SetActive(active);
}
