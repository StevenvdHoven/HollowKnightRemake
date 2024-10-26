#pragma once
#include "GUI.h"
#include <vector>

class GUIShape : public GUI
{
public:
	GUIShape(const Point2f& pos, const std::vector<Point2f>& relativevertices, const Color4f& color);
	GUIShape(const Rectf& rect, const Color4f& color, bool outline = false);

	void Draw() override;
	bool CheckOverUI(const Point2f& mousepos) override;
	std::vector<Point2f> MoveVertices(const Point2f& pos);

	void SetOpacity(float o);

private:
	Point2f m_Position;
	std::vector<Point2f> m_Vertices;
	Color4f m_Color;
	bool m_Outlined;
	
};

