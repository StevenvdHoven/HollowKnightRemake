#include "pch.h"
#include "PhyicsBody.h"
#include "Collider.h"
#include "PhysicsEngine.h"
#include "RectCollider.h"
#include <iostream>


PhyicsBody::PhyicsBody() :
	PhyicsBody(0, 0, new Collider(Point2f{ 0,0 }), true)
{
}

PhyicsBody::PhyicsBody(float mass, float bounce, bool isStatic, int layer) :
	m_ElpasedTime{ 0 },
	m_Mass{ mass },
	m_Bounce{ bounce },
	m_gravity{ gravity },
	m_Velocity{ Vector2f{0,0} },
	m_Collider{ nullptr },
	m_IsColliding{ false },
	m_IsStatic{ isStatic },
	m_BodyOnBodyCollision{ true },
	m_AppliedForces{}
{


}

PhyicsBody::~PhyicsBody()
{
	GetPhysicsEngine()->Delete(this);
}

void PhyicsBody::OnDelete()
{
	GetPhysicsEngine()->Delete(this);
}

void PhyicsBody::Init()
{

}

void PhyicsBody::Start()
{
	m_Collider = GetComponent<Collider>();
	GetPhysicsEngine()->Add(this);
}

void PhyicsBody::FixedUpdate(float elapsedSec)
{
	m_ElpasedTime = elapsedSec;
	if (m_IsStatic) {
		return;
	}

	if (m_Collider != nullptr) {
		m_Collider->SetPosition(GetGameObject()->GetPosition());
	}

	for (Vector2f force : m_AppliedForces) {
		m_Velocity += force * elapsedSec;
	}

	Point2f pos{ GetGameObject()->GetPosition() };
	Point2f futurePosX{ pos + Vector2f(m_Velocity.x, 0) * elapsedSec };
	Point2f futurePosY{ pos + Vector2f(0,m_Velocity.y) * elapsedSec };
	Collider* collidedX{ GetPhysicsEngine()->CheckCollision(m_Collider,futurePosX) };
	Collider* collidedY{ GetPhysicsEngine()->CheckCollision(m_Collider,futurePosY) };


	if (collidedX != nullptr && !collidedX->m_IsTrigger) {
		Simulate(collidedX, elapsedSec);
	}
	else {
		pos.x = futurePosX.x;
	}
	if (collidedY != nullptr && !collidedY->m_IsTrigger) {
		Simulate(collidedY, elapsedSec);
	}
	else {
		pos.y = futurePosY.y;
	}
	GetGameObject()->SetPosition(pos, collidedX == nullptr && collidedY == nullptr);



}

void PhyicsBody::Simulate(Collider* other, float elpasedTime)
{
	if (other->m_IsTrigger || m_Collider->m_IsTrigger)
	{
		return;
	}

	Point2f pos{ GetGameObject()->GetPosition() };

	PhyicsBody* body{ other->GetComponent<PhyicsBody>() };
	if (body != nullptr && !body->IsStatic())
	{
		if (!body->GetBodyOnBodyCollision() || !m_BodyOnBodyCollision) return;


		Vector2f relativeVelocity{ body->m_Velocity - m_Velocity };
		Vector2f pin{ m_Mass * m_Velocity + body->m_Mass * body->m_Velocity };
		float totalMass{ m_Mass + body->m_Mass };

		Vector2f body1{ (m_Mass - body->m_Mass) / (totalMass)*m_Velocity + (2 * body->m_Mass) / totalMass * body->m_Velocity };
		Vector2f body2{ (2 * m_Mass) / totalMass * m_Velocity + (body->m_Mass - m_Mass) / (totalMass)*body->m_Velocity };

		m_Velocity = body1;
		body->m_Velocity = body2;
	}
	else
	{
		utils::HitInfo info;
		if (utils::Raycast(other->GetPolygon(), pos, other->GetPosition(), info)) {
			m_Velocity.y = m_Velocity.Reflect(info.normal).y * m_Bounce;
			SubStepCollision(other, info.normal, elpasedTime);
		}
	}
}
void PhyicsBody::AddForce(Vector2f force, bool velocityChange)
{
	if (velocityChange) m_Velocity = force;
	else m_Velocity += force;
}

Point2f PhyicsBody::GetPerdictedPosition(float elpasedTime)
{
	Point2f pos{ GetGameObject()->GetPosition() };
	Vector2f futureVelocity = m_Velocity;
	return pos + futureVelocity * elpasedTime;
}

Point2f PhyicsBody::GetPerdictedPosition(const Vector2f& velocity, float elpasedTime)
{
	Point2f pos{ GetGameObject()->GetPosition() };
	Vector2f futureVelocity = velocity * elpasedTime;
	return pos + futureVelocity * elpasedTime;
}

void PhyicsBody::SetForces(std::vector<Vector2f> forces)
{
	m_AppliedForces = forces;
}

bool PhyicsBody::IsStatic()
{
	return m_IsStatic;
}

void PhyicsBody::SetBodyOnBodyCollision(bool collision)
{
	m_BodyOnBodyCollision = collision;
}

bool PhyicsBody::GetBodyOnBodyCollision()
{
	return m_BodyOnBodyCollision;
}

void PhyicsBody::SubStepCollision(Collider* other, Vector2f normal, float elapsedTime, int count)
{
	if (count > 10) {
		return;
	}

	GameObject* self{ GetGameObject() };
	Point2f pos{ self->GetPosition() };
	self->SetPosition(self->GetPosition() + normal * 2);

	if (other->CheckCollision(m_Collider))
	{
		SubStepCollision(other, normal, elapsedTime, count + 1);
	}
}

void PhyicsBody::OnCollisionEnter(Collider* collider)
{
	Simulate(collider, m_ElpasedTime);
}
