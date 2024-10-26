#pragma once
#include "GUI.h"
#include <vector>

enum TextAlign 
{
	Left,
	Center,
	Right
};

class Texture;
class GUIText : public GUI
{
public:
	GUIText(const Point2f pos, const std::string& text, TTF_Font* font, const Color4f& color,TextAlign alignment = TextAlign::Center);
	void OnDelete() override;

	void Draw() override;
	void SetText(const std::string& text, int MaxCharactersPerLine = -1);

private:
	void ClearLines();

	Point2f m_Position;
	std::string m_Text;
	std::vector<Texture*> m_Lines;
	TTF_Font* m_Font;
	Color4f m_Color;
	TextAlign m_Align;
};

