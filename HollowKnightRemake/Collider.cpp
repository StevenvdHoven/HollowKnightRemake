#include "pch.h"
#include "Collider.h"
#include "PhyicsBody.h"
#include "PhysicsEngine.h"


Collider::Collider(const Point2f& pos) :
	m_Position{ pos },
	m_Layer{ 0 },
	m_IsTrigger{ false }
{

}

Collider::~Collider()
{
	GetPhysicsEngine()->Delete(this);
}

void Collider::Init()
{
	GetPhysicsEngine()->Add(this);
}

void Collider::OnDelete()
{
	GetPhysicsEngine()->Delete(this);
}

bool Collider::CheckCollision(const Rectf& rect)
{
	return m_Position.x >= rect.left && m_Position.x <= rect.left + rect.width
		&& m_Position.y >= rect.bottom && m_Position.y <= rect.bottom + rect.height;
}

bool Collider::CheckCollision(const Circlef& circle)
{
	float distance = utils::GetDistance(m_Position, circle.center);
	return circle.radius <= distance;
}

bool Collider::CheckCollision(const std::vector<Point2f>& poly)
{
	return false;
}

bool Collider::CheckCollision(Collider* other)
{
	return false;
}

bool Collider::CheckCollision(Collider* other, const Point2f& futurePos)
{
	return false;
}

bool Collider::CheckWorldBounds(const Rectf& rect)
{
	return m_Position.x < rect.left || m_Position.x > rect.width
		|| m_Position.y < rect.bottom || m_Position.y > rect.height;
}

bool Collider::CheckWorldBounds(const Rectf& rect, const Point2f& futurePos)
{
	return futurePos.x < rect.left || futurePos.x > rect.width
		|| futurePos.y < rect.bottom || futurePos.y > rect.height;
}

Vector2f Collider::GetNormalFromBounds(const Rectf& rect)
{
	if (m_Position.x < rect.left) {
		return Vector2f{ 1,0 };
	}
	else if (m_Position.x > rect.width) {
		return Vector2f{ -1,0 };
	}
	else if (m_Position.y < rect.bottom) {
		return Vector2f{ 0,1 };
	}
	else if (m_Position.y > rect.height) {
		return Vector2f{ 0,-1 };
	}

	return Vector2f{ 0,0 };
}

Vector2f Collider::GetNormalFromBounds(const Rectf& rect, const Point2f& futurePos)
{
	if (futurePos.x < rect.left) {
		return Vector2f{ 1,0 };
	}
	else if (futurePos.x > rect.width) {
		return Vector2f{ -1,0 };
	}
	else if (futurePos.y < rect.bottom) {
		return Vector2f{ 0,1 };
	}
	else if (futurePos.y > rect.height) {
		return Vector2f{ 0,-1 };
	}

	return Vector2f{ 0,0 };
}

std::vector<Point2f> Collider::GetPolygon()
{
	return std::vector<Point2f>();
}

void Collider::SetPosition(const Point2f& pos)
{
	m_Position = pos;
}

Point2f Collider::GetPosition()
{
	return m_Position;
}

float Collider::GetHeight()
{
	return 0;
}

float Collider::GetWidth()
{
	return 0;
}

void Collider::Draw() const
{
	
}

void Collider::OnCollisionEnter(Collider* collider)
{
	ActivateEvent(collider);
}

void Collider::OnCollisionStay(Collider* collider)
{
	ActivateEvent(collider);
}

void Collider::OnCollisionExit(Collider* collider)
{
	ActivateEvent(collider);
}

void Collider::OnTriggerEnter(Collider* collider)
{
	ActivateEvent(collider);
}

void Collider::OnTriggerStay(Collider* collider)
{
	ActivateEvent(collider);
}

void Collider::OnTriggerExit(Collider* collider)
{
	ActivateEvent(collider);
}

void Collider::ActivateEvent(Collider* collider)
{
	for (int index{ 0 }; index < m_OnCollisions.size(); ++index)
	{
		if (m_OnCollisions[index] != nullptr)
		{
			m_OnCollisions[index](collider);
		}
	}
}

std::vector<Collider*> Collider::GetCollidingColliders()
{
	return m_CollidingColliders;
}

void Collider::AddCollidingCollider(Collider* collider)
{
	for (int index{ 0 }; index < m_CollidingColliders.size(); ++index) {
		if (m_CollidingColliders[index] == nullptr) {
			m_CollidingColliders[index] = collider;
			return;
		}
	}
	m_CollidingColliders.push_back(collider);
}

void Collider::RemoveCollidingCollider(Collider* collider)
{
	for (int index{ 0 }; index < m_CollidingColliders.size(); ++index) {
		if (collider == m_CollidingColliders[index]) {
			m_CollidingColliders[index] = nullptr;
			break;
		}
	}
	m_CollidingColliders.shrink_to_fit();
}
