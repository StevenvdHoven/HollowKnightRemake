#pragma once
#include <functional>
#include "GUI.h"

enum ButtonTypes 
{
	Down,
	Up,
	Both,
};

class Texture;
class Button : public GUI
{
public:
	Button(const Rectf& rect, const Color4f& color, const Color4f& textcolor,const std::string& displayText,TTF_Font* font,ButtonTypes type, std::function<void()> callback);
	Button(const Rectf& rect, Texture* text, const Color4f& textcolor, const std::string& displayText, TTF_Font* font, ButtonTypes type, std::function<void()> callback);
	void OnDelete() override;

	void Draw() override;
	void CheckOnClickDown(const Point2f& mousepos) override;
	void CheckOnClickUp(const Point2f& mousepos) override;
	bool CheckOverUI(const Point2f& mousepos) override;
	void SetText(const std::string& text);
	std::string GetText();

private:
	Rectf m_Rect;
	Texture* m_Texture;
	Color4f m_Color;
	Color4f m_TextColor;
	Texture* m_DisplayText;
	std::string m_Text;
	TTF_Font* m_Font;
	std::function<void()> m_CallBack;
	ButtonTypes m_Type;


	
	
};

