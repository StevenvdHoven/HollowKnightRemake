#include "pch.h"
#include "GUIText.h"
#include "Texture.h"

GUIText::GUIText(const Point2f pos, const std::string& text, TTF_Font* font, const Color4f& color, TextAlign alignment):
	m_Position{ pos },
	m_Text{ text},
	m_Font{ font },
	m_Color{ color },
	m_Align{ alignment }
{
	SetText(text);
}

void GUIText::OnDelete()
{
	ClearLines();
}


void GUIText::Draw()
{
	if (m_Lines.size() <= 0)
	{
		return;
	}

	for (int index{ 0 }; index < m_Lines.size(); ++index)
	{
		float width{ m_Lines[index]->GetWidth() };
		float height{ m_Lines[index]->GetHeight() };
		

		Rectf drawRect{
			m_Position.x - width * 0.5f,
			m_Position.y - height * 0.5f,
			(float)width,
			(float)height
		};

		if (m_Align == TextAlign::Left) {
			drawRect.left = m_Position.x;
			drawRect.bottom = m_Position.y;
		}
		else if (m_Align == TextAlign::Right) {
			drawRect.left = m_Position.x + width * 0.5f;
			drawRect.bottom = m_Position.y + height * 0.5f;
		}

		drawRect.bottom -= index * height;
		m_Lines[index]->Draw(drawRect);
	}
}


void GUIText::SetText(const std::string& text, int MaxCharactersPerLine)
{
	m_Text = text;
	if (m_Text == "") {
		ClearLines();
		return;
	}

	if (m_Lines.size() > 0) {
		ClearLines();
	}

	if (MaxCharactersPerLine == -1 || text.size() <= MaxCharactersPerLine)
	{
		m_Lines.push_back(new Texture(text, m_Font, m_Color));
	}
	else
	{
		int lines{ int(text.size() / MaxCharactersPerLine) };
		int remainder{ int(text.size() % MaxCharactersPerLine) };
		if (remainder != 0) {
			lines += 1;
		}

		int currentCharacterIndex{ 0 };
		const std::string content{ text };
		for (int index{ 0 }; index < lines; ++index)
		{
			std::string newContent(MaxCharactersPerLine, '\0');
			bool foundCharacter{ false };
			int contentIndex{ 0 };
			for (int charIndex{ currentCharacterIndex }; charIndex < currentCharacterIndex + MaxCharactersPerLine; ++charIndex)
			{
				if (content[charIndex] == ' ' && !foundCharacter)
				{
					continue;
				}
				else
				{
					foundCharacter = true;
				}
				
				if (charIndex >= m_Text.size()) 
				{
					break;
				}

				newContent[contentIndex] = content[charIndex];
				++contentIndex;
			}
			foundCharacter = false;
			currentCharacterIndex += MaxCharactersPerLine;


			m_Lines.push_back(new Texture{ newContent, m_Font, m_Color });
		}
		m_Lines.reserve(m_Lines.size());
	}

	
}

void GUIText::ClearLines()
{
	for (int index{ 0 }; index < m_Lines.size(); ++index)
	{
		delete m_Lines[index];
	}
	m_Lines.clear();
}
