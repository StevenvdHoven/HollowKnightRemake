#include "pch.h"
#include "SphereCollider.h"
#include "PhyicsBody.h"
#include "utils.h"

SphereCollider::SphereCollider(float radius, const Point2f& pos):
	Collider(pos),
	m_Radius{ radius }
{
}

bool SphereCollider::CheckCollision(const Rectf& rect)
{
	return utils::IsOverlapping(rect, Circlef(m_Position, m_Radius));
}

bool SphereCollider::CheckCollision(const Circlef& circle)
{
	return utils::IsOverlapping(circle, Circlef(m_Position, m_Radius));
}

bool SphereCollider::CheckCollision(const std::vector<Point2f>& poly)
{
	return utils::IsOverlapping(poly, Circlef(m_Position, m_Radius));
}

bool SphereCollider::CheckCollision(Collider* other)
{
	return other->CheckCollision(Circlef(m_Position, m_Radius));
}

bool SphereCollider::CheckCollision(Collider* other, const Point2f& futurePos)
{
	return other->CheckCollision(Circlef(futurePos, m_Radius));
}



bool SphereCollider::CheckWorldBounds(const Rectf& rect)
{
	return m_Position.x - m_Radius <= rect.left || m_Position.x + m_Radius >= rect.width
		|| m_Position.y - m_Radius <= rect.bottom || m_Position.y + m_Radius >= rect.height;
}

bool SphereCollider::CheckWorldBounds(const Rectf& rect, const Point2f& futurePos)
{
	return futurePos.x - m_Radius <= rect.left || futurePos.x + m_Radius >= rect.width
		|| futurePos.y - m_Radius <= rect.bottom || futurePos.y + m_Radius >= rect.height;
}

Vector2f SphereCollider::GetNormalFromBounds(const Rectf& rect)
{
	if (m_Position.x - m_Radius <= rect.left) {
		return Vector2f{ 1,0 };
	}
	else if (m_Position.x + m_Radius >= rect.width) {
		return Vector2f{ -1,0 };
	}
	else if (m_Position.y - m_Radius <= rect.bottom) {
		return Vector2f{ 0,1 };
	}
	else if (m_Position.y + m_Radius >= rect.height) {
		return Vector2f{ 0,-1 };
	}

	return Vector2f{ 0,0 };
}

Vector2f SphereCollider::GetNormalFromBounds(const Rectf& rect, const Point2f& futurePos)
{
	if (futurePos.x - m_Radius <= rect.left) {
		return Vector2f{ 1,0 };
	}
	else if (futurePos.x + m_Radius >= rect.width) {
		return Vector2f{ -1,0 };
	}
	else if (futurePos.y - m_Radius <= rect.bottom) {
		return Vector2f{ 0,1 };
	}
	else if (futurePos.y + m_Radius >= rect.height) {
		return Vector2f{ 0,-1 };
	}

	return Vector2f{ 0,0 };
}

std::vector<Point2f> SphereCollider::GetPolygon()
{
	std::vector<Point2f> points;
	for (int angle{ 0 }; angle < 360; ++angle) {
		float x{ cosf(angle * M_PI / 180.f) };
		float y{ sinf(angle * M_PI / 180.f) };
		points.push_back(Point2f(x, y));
	}
	return points;
}

float SphereCollider::GetHeight()
{
	return m_Radius;
}

float SphereCollider::GetWidth()
{
	return m_Radius;
}

void SphereCollider::Draw() const
{
	Collider::Draw();

	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::DrawEllipse(m_Position, m_Radius, m_Radius);
}
