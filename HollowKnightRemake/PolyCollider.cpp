#include "pch.h"
#include "PolyCollider.h"
#include "PhyicsBody.h"

PolyCollider::PolyCollider(const std::vector<Point2f>& verticis, const Point2f& pos):
    Collider(pos),
    m_Verticis{verticis},
    m_Width{0}
{
    m_Width = CaculateWidth();
}

bool PolyCollider::CheckCollision(const Rectf& rect)
{
    const std::vector<Point2f> verticics
    {
        Point2f(rect.left,rect.bottom),
        Point2f(rect.left,rect.bottom + rect.height),
        Point2f(rect.left + rect.width, rect.bottom + rect.height),
        Point2f(rect.left + rect.width, rect.bottom)
    };

    return CheckCollision(verticics);
}

bool PolyCollider::CheckCollision(const Circlef& circle)
{
    return utils::IsOverlapping(m_Verticis,circle);
}

bool PolyCollider::CheckCollision(const std::vector<Point2f>& poly)
{
    for (int index{ 0 }; index < poly.size(); ++index) {
        if (utils::IsPointInPolygon(poly[index], m_Verticis)) {
            return true;
        }
    }

    return false;
}

bool PolyCollider::CheckCollision(Collider* other)
{
    return other->CheckCollision(m_Verticis);
}

bool PolyCollider::CheckCollision(Collider* other, const Point2f& futurePos)
{
    return other->CheckCollision(GetWorldVerticis(futurePos));
}

bool PolyCollider::CheckWorldBounds(const Rectf& rect)
{
    return CheckWorldBounds(rect,m_Position);
}

bool PolyCollider::CheckWorldBounds(const Rectf& rect, const Point2f& futurePos)
{
    const std::vector<Point2f> worldVerticis{ GetWorldVerticis(futurePos) };
    for (int index{ 0 }; index < worldVerticis.size(); ++index) {
        if (!utils::IsPointInRect(worldVerticis[index], rect)) {
            return true;
        }
    }
    return false;
}

Vector2f PolyCollider::GetNormalFromBounds(const Rectf& rect)
{
    return Vector2f();
}

Vector2f PolyCollider::GetNormalFromBounds(const Rectf& rect, const Point2f& futurePos)
{
    return Vector2f();
}

std::vector<Point2f> PolyCollider::GetPolygon()
{
    return m_Verticis;
}

float PolyCollider::GetHeight()
{
    return m_Width;
}

float PolyCollider::GetWidth()
{
    return m_Width;
}

void PolyCollider::Draw() const
{
    Collider::Draw();

    utils::SetColor(Color4f(0, 1, 1, 1));
	utils::DrawPoint(m_Position, 5);
    utils::SetColor(Color4f(1, 0, 0, 1));
    utils::DrawPolygon(m_Verticis);
}

float PolyCollider::CaculateWidth()
{
    float longestDistance{ -1 };
    for (int first{ 0 }; first < m_Verticis.size(); ++first) {
        for (int second{ 0 }; second < m_Verticis.size(); ++second) {
            if (first == second) {
                continue;
            }
            float distance{ utils::GetDistance(m_Verticis[first],m_Verticis[second]) };
            if (distance > longestDistance) {
                longestDistance = distance;
            }
        }
    }
    return longestDistance;
}

std::vector<Point2f> PolyCollider::GetWorldVerticis(const Point2f& position)
{
    Transform trans;
    trans.Position = Vector2f(position);
    return utils::ApplyMatrix(trans,m_Verticis);
}


