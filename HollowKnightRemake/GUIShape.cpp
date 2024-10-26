#include "pch.h"
#include "GUIShape.h"
#include <utils.h>
#include "GUIManager.h"

GUIShape::GUIShape(const Point2f& pos, const std::vector<Point2f>& relativevertices, const Color4f& color) :
	m_Position{ pos },
	m_Vertices{ relativevertices },
	m_Color{ color },
	m_Outlined {false}
{
}

GUIShape::GUIShape(const Rectf& rect, const Color4f& color, bool outline) :
	GUIShape(Point2f{ 0,0 }, std::vector<Point2f>
{
		Point2f{ rect.left,rect.bottom },
		Point2f{ rect.left ,rect.bottom + rect.height },
		Point2f{ rect.left + rect.width ,rect.bottom + rect.height },
		Point2f{ rect.left + rect.width ,rect.bottom },
}, color)
{
	m_Outlined = outline;
}

void GUIShape::Draw()
{
	std::vector<Point2f> convertedVertices{ MoveVertices(m_Position) };
	utils::SetColor(m_Color);
	utils::FillPolygon(convertedVertices);
	utils::SetColor(black);
	utils::DrawPolygon(convertedVertices);
}

bool GUIShape::CheckOverUI(const Point2f& mousepos)
{
	Transform trans;
	trans.Position = Vector2f(m_Position);

	return utils::IsPointInPolygon(mousepos,utils::ApplyMatrix(trans,m_Vertices));
}

std::vector<Point2f> GUIShape::MoveVertices(const Point2f& pos)
{
	std::vector<Point2f> convertedVertices;
	for (Point2f vertex : m_Vertices) {
		convertedVertices.push_back(vertex + Vector2f(pos.x,pos.y));
	}
	return convertedVertices;
}

void GUIShape::SetOpacity(float o)
{
	m_Color.a = o;
}
