#include "pch.h"
#include "GUIToggle.h"
#include "utils.h"

GUIToggle::GUIToggle(const Rectf& rect, const Color4f& bgColor, const Color4f& fillColor, std::function<void(bool)> callback):
	m_Rect{ rect },
	m_Center{ rect.left + rect.width * 0.5f, rect.bottom + rect.height * 0.5f},
	m_BackgroundColor{ bgColor },
	m_FillColor{ fillColor },
	m_CallBack{ callback },
	m_Toggled{ false }
{
}

void GUIToggle::CheckOnClickUp(const Point2f& mousepos)
{
	if (utils::IsPointInRect(mousepos, m_Rect)) 
	{
		m_Toggled = !m_Toggled;
		m_CallBack(m_Toggled);
	}
}

void GUIToggle::Draw()
{
	utils::SetColor(m_BackgroundColor);
	utils::FillRect(m_Rect);
	
	if (m_Toggled) 
	{
		utils::SetColor(m_FillColor);
		utils::FillEllipse(m_Center, m_Rect.width * 0.45f, m_Rect.height * 0.45f);
	}
}

void GUIToggle::SetValue(bool value)
{
	m_Toggled = value;
}
