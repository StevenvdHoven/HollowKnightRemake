#include "pch.h"
#include "GUIinputTab.h"
#include "utils.h"
#include "GUIManager.h"

bool GUIinputTab::m_Typing{ false };

GUIinputTab::GUIinputTab(const Rectf& rect,TTF_Font* font, const Color4f& color, const Color4f& textcolor, std::function<void(const std::string& value)> callback):
	m_Rect{ rect },
	m_Color{ color },
	m_Button{ nullptr },
	m_Activated{ false },
	m_Input{},
	m_OnValued{callback}
{
	std::function<void()> onClickUp{ [this]()
		{
			m_Activated = true;
			GUIinputTab::m_Typing = true;
		}
	};

	m_Button = new Button(
		rect,
		color,
		textcolor,
		" ",
		font,
		ButtonTypes::Up,
		onClickUp);

	m_Button->SetParent(this);
}


void GUIinputTab::OnKeyDown(const SDL_KeyboardEvent& e)
{
	if (!m_Activated) {
		return;
	}

	if (e.keysym.sym == SDLK_BACKSPACE || e.keysym.sym == 13)
	{
		return;
	}

	m_Input += (char)e.keysym.sym;
	m_Button->SetText(m_Input);

}

void GUIinputTab::OnKeyUp(const SDL_KeyboardEvent& e)
{
	if (!m_Activated) {
		return;
	}

	if (e.keysym.sym == SDLK_BACKSPACE)
	{
		if (m_Input.length() > 0) 
		{
			m_Input.pop_back();
			m_Button->SetText(m_Input);
		}
	}
	if (e.keysym.sym == 13)
	{
		m_Activated = false;
		GUIinputTab::m_Typing = false;
		m_OnValued(m_Input);
		return;
	}
}

void GUIinputTab::OnParent(GUI* parent)
{
	m_Button->SetManager(parent->GetManager());
}

bool GUIinputTab::CheckOverUI(const Point2f& mousepos)
{
	return utils::IsPointInRect(mousepos,m_Rect);
}

void GUIinputTab::SetValue(const std::string& value, int maxCharacters)
{
	std::string content{ value };
	if (maxCharacters != -1 && maxCharacters != 0)
	{
		std::string newContent(maxCharacters,'\0');
		for (int index{ 0 }; index < maxCharacters; ++index)
		{
			newContent[index] = content[index];
		}
		content = newContent;
	}
	
	m_Input = content;
	m_Button->SetText(m_Input);
}

bool GUIinputTab::IsTyping()
{
	return m_Activated;
}
