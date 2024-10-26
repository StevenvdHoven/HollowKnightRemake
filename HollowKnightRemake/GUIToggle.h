#pragma once
#include "GUI.h"
#include <functional>

class GUIToggle : public GUI
{
public:
	GUIToggle(const Rectf& rect, const Color4f& bgColor, const Color4f& fillColor, std::function<void(bool)> callback);

	void CheckOnClickUp(const Point2f& mousepos) override;
	void Draw() override;
	
	void SetValue(bool value);

private:
	Rectf m_Rect;
	Point2f m_Center;
	Color4f m_BackgroundColor;
	Color4f m_FillColor;
	std::function<void(bool)> m_CallBack;

	bool m_Toggled;
};

