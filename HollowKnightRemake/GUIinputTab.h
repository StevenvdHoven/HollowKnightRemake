#pragma once
#include "GUI.h"
#include "Button.h"
class GUIinputTab final : public GUI
{
public:
	GUIinputTab(const Rectf& rect, TTF_Font* font, const Color4f& color, const Color4f& textcolor, std::function<void(const std::string& value)> callback);

	void OnKeyDown(const SDL_KeyboardEvent& e) override;
	void OnKeyUp(const SDL_KeyboardEvent& e) override;

	void OnParent(GUI* parent) override;

	bool CheckOverUI(const Point2f& mousepos) override;

	void SetValue(const std::string& value, int maxCharacters = -1);

	bool IsTyping();

	static bool m_Typing;

private:
	Rectf m_Rect;
	Color4f m_Color;

	Button* m_Button;
	bool m_Activated;

	std::string m_Input;
	std::function<void(const std::string& value)> m_OnValued;

	
};

