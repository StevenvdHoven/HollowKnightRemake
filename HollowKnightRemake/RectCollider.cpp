#include "pch.h"
#include "RectCollider.h"
#include "PhyicsBody.h"

RectCollider::RectCollider(float width, float height, const Point2f& pos):
	Collider(pos),
	m_Width{width},
	m_Height{height}
{
}

bool RectCollider::CheckCollision(const Rectf& rect)
{
	return utils::IsOverlapping(rect,
		Rectf
		{
			m_Position.x - m_Width * 0.5f,
			m_Position.y - m_Height * 0.5f,
			m_Width,
			m_Height

		});
}

bool RectCollider::CheckCollision(const Circlef& circle)
{
	return utils::IsOverlapping(
		Rectf
		{
			m_Position.x - m_Width * 0.5f,
			m_Position.y - m_Height * 0.5f,
			m_Width,
			m_Height

		}, circle);
}

bool RectCollider::CheckCollision(Collider* other)
{
	return other->CheckCollision(
		Rectf
		{
			m_Position.x - m_Width * 0.5f,
			m_Position.y - m_Height * 0.5f,
			m_Width,
			m_Height
		});
}

bool RectCollider::CheckCollision(Collider* other, const Point2f& futurePos)
{
	return other->CheckCollision(
		Rectf
		{
			futurePos.x - m_Width * 0.5f,
			futurePos.y - m_Height * 0.5f,
			m_Width,
			m_Height
		});
}

bool RectCollider::CheckWorldBounds(const Rectf& rect)
{
	const float halfWidth{ m_Width * 0.5f };
	const float halfHeight{ m_Height * 0.5f };
	return m_Position.x - halfWidth <= rect.left || m_Position.x + halfWidth >= rect.width
		|| m_Position.y - halfHeight <= rect.bottom || m_Position.y + halfHeight >= rect.height;
}

bool RectCollider::CheckWorldBounds(const Rectf& rect, const Point2f& futurePos)
{
	const float halfWidth{ m_Width * 0.5f };
	const float halfHeight{ m_Height * 0.5f };
	return futurePos.x - halfWidth <= rect.left || futurePos.x + halfWidth >= rect.width
		|| futurePos.y - halfHeight <= rect.bottom || futurePos.y + halfHeight >= rect.height;
}

Vector2f RectCollider::GetNormalFromBounds(const Rectf& rect)
{
	if (m_Position.x - m_Width * 0.5f <= rect.left) {
		return Vector2f{ 1,0 };
	}
	else if (m_Position.x + m_Width * 0.5f >= rect.width) {
		return Vector2f{ -1,0 };
	}
	else if (m_Position.y - m_Height * 0.5f <= rect.bottom) {
		return Vector2f{ 0,1 };
	}
	else if (m_Position.y + m_Height * 0.5f >= rect.height) {
		return Vector2f{ 0,-1 };
	}

	return Vector2f{ 0,0 };
}

Vector2f RectCollider::GetNormalFromBounds(const Rectf& rect, const Point2f& futurePos)
{
	if (futurePos.x - m_Width*0.5f <= rect.left) {
		return Vector2f{ 1,0 };
	}
	else if (futurePos.x + m_Width *0.5f >= rect.width) {
		return Vector2f{ -1,0 };
	}
	else if (futurePos.y - m_Height*0.5f <= rect.bottom) {
		return Vector2f{ 0,1 };
	}
	else if (futurePos.y + m_Height*0.5f >= rect.height) {
		return Vector2f{ 0,-1 };
	}

	return Vector2f{ 0,0 };
}

std::vector<Point2f> RectCollider::GetPolygon()
{
	return std::vector<Point2f>{
		Point2f{m_Position.x - m_Width * 0.5f, m_Position.y - m_Height * 0.5f},
		Point2f{ m_Position.x - m_Width * 0.5f,  m_Position.y + m_Height * 0.5f },
		Point2f{ m_Position.x + m_Width * 0.5f, m_Position.y + m_Height * 0.5f },
		Point2f{ m_Position.x + m_Width * 0.5f, m_Position.y - m_Height * 0.5f },
	};
}

float RectCollider::GetHeight()
{
	return m_Height;
}

float RectCollider::GetWidth()
{
	return m_Width;
}

void RectCollider::Draw() const
{
	Collider::Draw();

	utils::SetColor(Color4f(0, 1, 1, 1));
	utils::DrawPoint(GetGameObject()->GetPosition(), 5);

	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::DrawRect(Rectf
		{
			GetGameObject()->GetPosition().x - m_Width * 0.5f,
			GetGameObject()->GetPosition().y - m_Height * 0.5f,
			m_Width,
			m_Height,
		});
}

void RectCollider::SetWidth(float width)
{
	m_Width = width;
}

void RectCollider::SetHeight(float height)
{
	m_Height = height;
}

Rectf RectCollider::GetRect()
{
	return Rectf{
		m_Position.x - m_Width * 0.5f, 
		m_Position.y - m_Height * 0.5f,
		m_Width,
		m_Height
	};
}
