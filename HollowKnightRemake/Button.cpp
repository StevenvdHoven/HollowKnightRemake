#include "pch.h"
#include "Button.h"
#include "Texture.h"
#include "utils.h"
#include "GUIManager.h"



Button::Button(const Rectf& rect, const Color4f& color, const Color4f& textcolor, const std::string& displayText, TTF_Font* font, ButtonTypes type, std::function<void()> callback) :
	GUI(),
m_Rect{ rect },
m_Texture{nullptr},
m_Color{color},
m_TextColor{textcolor},
m_DisplayText{new Texture(displayText,font,textcolor)},
m_Text{displayText},
m_Font{font},
m_Type{ type },
m_CallBack{callback}
{
}

Button::Button(const Rectf& rect, Texture* text, const Color4f& textcolor, const std::string& displayText, TTF_Font* font, ButtonTypes type, std::function<void()> callback) :
	GUI(),
	m_Rect{ rect },
	m_Texture{ text },
	m_Color{ Color4f{0,0,0,0} },
	m_TextColor{ m_Color },
	m_DisplayText{ new Texture(displayText,font,textcolor) },
	m_Text{displayText},
	m_Font{font},
	m_Type{ type },
	m_CallBack{ callback }
{
}

void Button::OnDelete()
{
	delete m_DisplayText;
	m_DisplayText = nullptr;
	m_Font = nullptr;
}

void Button::Draw()
{
	if (m_Texture == nullptr) {
		utils::SetColor(m_Color);
		utils::FillRect(m_Rect);
	}
	else {
		m_Texture->Draw(m_Rect);
	}

	const Point2f center{ m_Rect.left + m_Rect.width * 0.5f,m_Rect.bottom + m_Rect.height * 0.5f };

	int width, height;
	TTF_SizeText(m_Font, m_Text.c_str(), &width, &height);

	Rectf drawRect{
		center.x - width * 0.5f,
		center.y - height * 0.5f,
		(float)width,
		(float)height
	};
	m_DisplayText->Draw(drawRect);
}

void Button::CheckOnClickDown(const Point2f& mousepos)
{
	GUIManager* manager{ GetManager() };
	if (utils::IsOverlapping(manager->GetMousePos(), m_Rect) &&
		(m_Type == ButtonTypes::Down || m_Type == ButtonTypes::Both)) {
		m_CallBack();
	}
}

void Button::CheckOnClickUp(const Point2f& mousepos)
{
	if (utils::IsOverlapping(GetManager()->GetMousePos(), m_Rect) &&
		(m_Type == ButtonTypes::Up || m_Type == ButtonTypes::Both)) {
		m_CallBack();
	}
}

bool Button::CheckOverUI(const Point2f& mousepos)
{
	return utils::IsPointInRect(mousepos,m_Rect);
}

void Button::SetText(const std::string& text)
{
	m_Text = text;
	if (m_Text == "") {
		m_Text = " ";
	}

	if (m_DisplayText != nullptr) {
		delete m_DisplayText;
		m_DisplayText = new Texture(text, m_Font, m_TextColor);
		m_Text = text;
	}
}

std::string Button::GetText()
{
	return m_Text;
}
