#include "pch.h"
#include "SnippingTool.h"
#include "utils.h"
#include "Texture.h"
#include "MapBuilder.h"
#include "MapObject.h"
#include "Transform.h"
#include <iostream>
#include "GUI.h"
#include "TextureManager.h"
#include "GUIManager.h"

SnippingTool::SnippingTool(const Rectf& viewPort,TextureManager* manager, GUIManager* guiManager, std::function<void(MapObject*)> func) :
	m_ViewPort{ viewPort },
	m_ExportCallBack{func},
	m_Position{ 1920 - 200,0 },
	m_StartSnipPosition{0,0},
	m_EndSnipPosition{0,0},
	m_CurrentBounds{},
	m_CurrentLoadedTexture{nullptr},
	m_CurrentTextureIndex{-1},
	m_PanelSpacing{15},
	m_TextureManager{manager},
	m_GUIManager{ guiManager},
	m_Options{},
	m_Font{nullptr},
	m_CuttingIcon{new Texture("Icons/scissors.png")},
	m_EraseIcon{new Texture("Icons/eraser.png")},
	m_ListIcon{new Texture("Icons/list.png")}
{
	m_Font = m_GUIManager->GetFont(arial_14);
	LoadOptions();
}

SnippingTool::~SnippingTool()
{
	delete m_CuttingIcon;
	delete m_EraseIcon;
	delete m_ListIcon;
}

void SnippingTool::OnClick(const Point2f& mousePos)
{
	if (m_CurrentLoadedTexture != nullptr) {
		if (CheckForButtonClick(mousePos)) {
			return;
		}
	}

	m_StartSnipPosition = mousePos;
	m_Dragging = true;
}

void SnippingTool::OnMouseMotion(const Point2f& mousePos)
{
	if (m_Dragging)
	{
		m_EndSnipPosition = mousePos;
	}
}

void SnippingTool::OnClickUp(const Point2f& mousePos)
{
	if (!m_Dragging) {
		return;
	}

	if (m_CurrentLoadedTexture == nullptr)
	{
		const int clickedButtonIndex{ CheckForOptionClick(mousePos) };
		if (clickedButtonIndex == -1) {
			return;
		}
		m_CurrentLoadedTexture = m_TextureManager->GetTexture(clickedButtonIndex);
		m_CurrentTextureIndex = clickedButtonIndex;
	}

	m_EndSnipPosition = mousePos;
	m_Dragging = false;
}

void SnippingTool::Draw()
{
	DrawPanel();
	DrawOptions();
	DrawButtons();
	DrawCuttingRect();
}

bool SnippingTool::IsMouseOverSnippingTool(const Point2f& mousePos)
{
	Rectf rect;
	if (m_CurrentLoadedTexture == nullptr)
	{
		rect = Rectf
		{
			m_Position.x,
			m_Position.y,
			200,
			500
		};
	}
	else
	{
		const Point2f drawPos{ 1920 - 600, 0 };
		rect = Rectf
		{
			drawPos.x,
			drawPos.y,
			600,
			600
		};
	}

	return utils::IsPointInRect(mousePos, rect);
}

void SnippingTool::LoadOptions()
{
	m_Options = m_TextureManager->GetFilePaths();
	m_EffectOptions = std::vector<std::string>
	{
		"RoomEffects01.png",
	};
}

void SnippingTool::CloseTexture()
{
	m_CurrentLoadedTexture = nullptr;
	m_CurrentTextureIndex = -1;
}

void SnippingTool::Clear()
{
	m_StartSnipPosition = Point2f();
	m_EndSnipPosition = Point2f();
}

void SnippingTool::Export()
{
	m_ExportCallBack(new MapObject{ -1, m_CurrentTextureIndex, Point2f{m_ViewPort.width * 0.5f,m_ViewPort.height * 0.5f}, CaculateBounds(),m_TextureManager });
}

int SnippingTool::CheckForOptionClick(const Point2f& mousePos)
{
	const float
		width{ 180 },
		height{ 20 };

	Point2f drawPosition{ m_Position + Vector2f{10,10} };

	for (int index{ 0 }; index < m_Options.size(); ++index)
	{
		if(mousePos.x >= drawPosition.x && mousePos.x <= drawPosition.x + width)
		{
			if(mousePos.y >= drawPosition.y && mousePos.y <= drawPosition.y + height)
			{
				return index;
			}
		}

		drawPosition.y += height;
	}

	return -1;
}

bool SnippingTool::CheckForButtonClick(const Point2f& mousePos)
{
	const Point2f topLeft{ 1920 - 600,600 };
	const Rectf cuttingToolBounds{
		topLeft.x + 10,
		topLeft.y - 60,
		50,
		50
	};

	const Rectf eraseToolBound{
		topLeft.x + 60,
		topLeft.y - 60,
		50,
		50
	};

	const Rectf listToolBound{
		1920 - 60,
		600 - 60,
		50,
		50
	};

	if (PointInRect(cuttingToolBounds, mousePos)) {
		Export();
		Clear();
		return true;
	}
	else if (PointInRect(eraseToolBound, mousePos)) {
		Clear();
		return true;
	}
	else if (PointInRect(listToolBound, mousePos)) {
		CloseTexture();
		return true;
	}

	return false;
}

bool SnippingTool::PointInRect(const Rectf& rect, const Point2f& point)
{
	if (point.x >= rect.left && point.x <= rect.left + rect.width) {
		if (point.y >= rect.bottom && point.y <= rect.bottom + rect.height) {
			return true;
		}
	}

	return false;
}

void SnippingTool::DrawOptions()
{
	if (m_CurrentLoadedTexture != nullptr) {
		return;
	}

	const float
		width{ 180 },
		height{ 20 };

	Point2f drawPosition{ m_Position + Vector2f{10,10} };

	for (int index{ 0 }; index < m_Options.size(); ++index)
	{
		utils::SetColor(Color4f{ .2f,.2f,.2f,1.f });
		utils::FillRect(drawPosition, width, height);
		utils::SetColor(Color4f{ 1.f, 0 ,0 ,1.f });
		utils::DrawRect(drawPosition, width, height);

		Texture text{ m_Options[index],m_Font,Color4f(1,1,1,1) };
		text.Draw(drawPosition);

		drawPosition.y += height;
	}
}

void SnippingTool::DrawButtons()
{
	if (m_CurrentLoadedTexture == nullptr) 
	{
		return;
	}

	const Point2f topLeft{ 1920 - 600,600 };
	m_CuttingIcon->Draw(Rectf(topLeft.x + 10, topLeft.y - 60, 50, 50));
	m_EraseIcon->Draw(Rectf(topLeft.x + 60, topLeft.y - 60, 50, 50));
	m_ListIcon->Draw(Rectf(1920 - 60, 600 - 60, 50, 50));
}

void SnippingTool::DrawPanel()
{
	utils::SetColor(Color4f{ 1,1,1,1 });
	if (m_CurrentLoadedTexture == nullptr) 
	{
		utils::FillRect(m_Position, 200, 500);
	}
	else 
	{
		const Point2f drawPos{ 1920 - 600, 0 };
		utils::FillRect(drawPos, 1920 - drawPos.x, 600);

		DrawTexture();
	}
}

void SnippingTool::DrawTexture()
{
	if (m_CurrentLoadedTexture == nullptr) 
	{
		return;
	}

	Transform transform;
	bool effect{ false };
	for (int index{ 0 }; index < m_EffectOptions.size(); ++index)
	{
		if (m_EffectOptions[index] == m_Options[m_CurrentTextureIndex])
		{
			effect = true;
		}
	}


	Point2f drawPosition{ 1920 - 580, m_PanelSpacing };
	const float widthScaleFactor{ 500 / m_CurrentLoadedTexture->GetWidth() };
	const float heightScaleHeight{ 500 / m_CurrentLoadedTexture->GetHeight() };
	
	transform.Position = Vector2f(drawPosition.x, drawPosition.y);

	transform.ApplyTransformation();

	if (effect)
	{
		utils::SetColor(Color4f{ 0,0,0,1 });
		utils::FillRect(0, 0, 500, 500);
	}

	const float height{ m_CurrentLoadedTexture->GetHeight() * heightScaleHeight };
	m_CurrentLoadedTexture->Draw(Rectf(0,0,m_CurrentLoadedTexture->GetWidth() * widthScaleFactor, height));
	utils::SetColor(Color4f(1, 0, 0, 1));
	utils::DrawRect(0, 0, 500, 500);
	transform.ResetTransformation();
}

void SnippingTool::DrawCuttingRect()
{
	if (m_CurrentLoadedTexture == nullptr) {
		return;
	}

	utils::SetColor(Color4f(0.f, 1.f, 0.f, 1.f));
	Vector2f direction = m_EndSnipPosition - m_StartSnipPosition;

	utils::DrawRect(m_StartSnipPosition.x, m_StartSnipPosition.y, direction.x, direction.y);

}

Rectf SnippingTool::CaculateBounds()
{
	const float widthRescaleFactor{ m_CurrentLoadedTexture->GetWidth() / 500 };
	const float heightRescaleFactor{ m_CurrentLoadedTexture->GetHeight() / 500 };
	const Rectf rect
	{
		(m_StartSnipPosition.x - (1920 - 580))* widthRescaleFactor,
		m_CurrentLoadedTexture->GetHeight() - (m_StartSnipPosition.y - m_PanelSpacing) * heightRescaleFactor,
		abs(m_EndSnipPosition.x - m_StartSnipPosition.x) * widthRescaleFactor,
		abs(m_EndSnipPosition.y - m_StartSnipPosition.y) * heightRescaleFactor,
	};

	std::cout << rect.bottom << std::endl;
	return rect;
}
