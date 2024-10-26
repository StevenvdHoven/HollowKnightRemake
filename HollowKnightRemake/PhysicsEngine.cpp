#include "pch.h"
#include "utils.h"
#include "PhysicsEngine.h"
#include "Collider.h"
#include "PhyicsBody.h"
#include "GameObject.h"




PhysicsEngine::PhysicsEngine():
	m_Colliders{},
	m_Bodies{}
{
}

PhysicsEngine::~PhysicsEngine()
{
	m_Bodies.clear();
	m_Colliders.clear();
}

void PhysicsEngine::Update(float elapsedSec)
{
	//CheckCollisions(elapsedSec);

	for (int index{ 0 }; index < m_Bodies.size(); ++index) {
		if (m_Bodies[index] != nullptr && m_Bodies[index]->IsEnabled()) 
		{
			m_Bodies[index]->AddForce(Vector2f{0.f,m_Bodies[index]->m_Mass * (-m_Bodies[index]->m_gravity) * elapsedSec});
			Collider* bodyCollider{ m_Bodies[index]->GetComponent<Collider>() };

			for(int second{0}; second < m_Colliders.size(); ++second)
			{
				if (m_Colliders[second] == nullptr || !m_Colliders[second]->IsEnabled())
				{
					continue;
				}

				bool colliding = bodyCollider->CheckCollision(m_Colliders[second], m_Bodies[index]->GetPerdictedPosition(elapsedSec));
				CheckCollsionEventType(bodyCollider, m_Colliders[second], colliding);

				if (m_Colliders[second] != nullptr && !m_Colliders[second]->IsEnabled() && colliding && !bodyCollider->m_IsTrigger && !m_Colliders[second]->m_IsTrigger)
				{
					utils::HitInfo info;

					if (utils::Raycast(m_Colliders[second]->GetPolygon(),
						m_Bodies[index]->GetGameObject()->GetPosition(),
						m_Colliders[second]->GetPosition(), info
					))
					{
						m_Bodies[index]->AddForce(info.normal * m_Bodies[index]->m_Mass * m_Bodies[index]->m_gravity * elapsedSec);
					}
				}

			}
			m_Bodies[index]->FixedUpdate(elapsedSec);
		}
	}
}

void PhysicsEngine::CheckCollsionEventType(Collider* first, Collider* second, bool colliding, bool repeated)
{

	if (colliding) {

		std::vector<Collider*> alreadyCollided{ first->GetCollidingColliders() };
		for (int index{ 0 }; index < alreadyCollided.size(); ++index) {
			if (alreadyCollided[index] == second) {
				if (second->m_IsTrigger) 
				{
					first->GetGameObject()->OnTriggerStay(second);
				}
				else 
				{
					first->GetGameObject()->OnCollisionStay(second);
				}
				
				return;
			}
		}

		first->AddCollidingCollider(second);
		if (second->m_IsTrigger)
		{
			first->GetGameObject()->OnTriggerEnter(second);
		}
		else 
		{
			first->GetGameObject()->OnCollisionEnter(second);
		}
	}
	else {
		std::vector<Collider*> colliders{ first->GetCollidingColliders() };
		bool has{ false };
		for (int index{ 0 }; index < colliders.size(); ++index) {
			if (colliders[index] == second) 
			{
				has = true;
				break;
			}
		}

		if (!has) 
		{
			return;
		}

		first->RemoveCollidingCollider(second);
		if (second->m_IsTrigger)
		{
			first->GetGameObject()->OnTriggerExit(second);
		}
		else 
		{
			first->GetGameObject()->OnCollisionExit(second);
		}
	}

	if (!repeated)
	{
		CheckCollsionEventType(second, first, colliding, true);
	}
}

Collider* PhysicsEngine::CheckCollision(Collider* collider)
{
	PhyicsBody* firstBody{ collider->GetComponent<PhyicsBody>() };
	
	for (int first{ 0 }; first < m_Colliders.size(); ++first)
	{
		if (m_Colliders[first] == nullptr
			|| collider == m_Colliders[first]
			|| !m_Colliders[first]->IsEnabled())
		{
			continue;
		}

		if (m_Colliders[first]->m_IsTrigger
			|| collider->m_IsTrigger)
		{
			continue;
		}

		PhyicsBody* secondBody{ m_Colliders[first]->GetComponent<PhyicsBody>() };
		if (firstBody != nullptr && secondBody != nullptr) {
			if (!firstBody->GetBodyOnBodyCollision())
			{
				continue;
			}
		}

		if (collider->CheckCollision(m_Colliders[first]))
		{
			return m_Colliders[first];
		}

	}
	return nullptr;
}

Collider* PhysicsEngine::CheckCollision(Collider* collider, const Point2f& futurePos)
{
	PhyicsBody* firstBody{ collider->GetComponent<PhyicsBody>() };

	for (int first{ 0 }; first < m_Colliders.size(); ++first)
	{
		

		if (m_Colliders[first] == nullptr 
			|| collider == m_Colliders[first]
			|| !m_Colliders[first]->IsEnabled())
		{
			continue;
		}

		if (m_Colliders[first]->m_IsTrigger
			|| collider->m_IsTrigger) 
		{
			continue;
		}

		PhyicsBody* secondBody{ m_Colliders[first]->GetComponent<PhyicsBody>() };
		if (firstBody != nullptr && secondBody != nullptr) {
			if (!firstBody->GetBodyOnBodyCollision() || !secondBody->GetBodyOnBodyCollision())
			{
				continue;
			}
		}

		if (collider->CheckCollision(m_Colliders[first],futurePos))
		{
			return m_Colliders[first];
		}
	}
	return nullptr;
}

void PhysicsEngine::Add(Collider* collider)
{
	for (int index{ 0 }; index < m_Colliders.size(); ++index)
	{
		if (m_Colliders[index] == nullptr) {
			m_Colliders[index] = collider;
			return;
		}
	}
	m_Colliders.push_back(collider);
}

void PhysicsEngine::Add(PhyicsBody* body)
{
	for (int index{ 0 }; index < m_Bodies.size(); ++index)
	{
		if (m_Bodies[index] == nullptr) {
			m_Bodies[index] = body;
			return;
		}
	}
	m_Bodies.push_back(body);
}

void PhysicsEngine::Delete(Collider* collider)
{
	for (int index{ 0 }; index < m_Colliders.size(); ++index)
	{
		if (m_Colliders[index] == collider) {
			m_Colliders[index] = nullptr;
			return;
		}
	}
}

void PhysicsEngine::Delete(PhyicsBody* body)
{
	for (int index{ 0 }; index < m_Bodies.size(); ++index)
	{
		if (m_Bodies[index] == body) {
			m_Bodies[index] = nullptr;
			return;
		}
	}
}

bool PhysicsEngine::Raycast(const Point2f& orgin, const Point2f& end, utils::HitInfo& info, Collider** collider, const std::vector<Layers> layers, Collider* self)
{
	for (int index{ 0 }; index < m_Colliders.size(); ++index) 
	{
		if (m_Colliders[index] == nullptr 
			|| m_Colliders[index] == self 
			|| !m_Colliders[index]->IsEnabled() 
			|| !ContainsLayer((Layers)m_Colliders[index]->m_Layer, layers)) 
		{
			continue;
		}

		if (utils::Raycast(m_Colliders[index]->GetPolygon(), orgin, end, info)) {
			
			*collider = m_Colliders[index];
			return true;
		}
	}
	return false;
}

bool PhysicsEngine::RayCastAll(const Point2f& orgin, const Point2f& end, std::vector<utils::HitInfo>& info, std::vector<Collider*>& colliders, const std::vector<Layers> layers, Collider* self)
{
	bool hit{ false };

	for (int index{ 0 }; index < m_Colliders.size(); ++index)
	{
		if (m_Colliders[index] == nullptr 
			|| m_Colliders[index] == self 
			|| !m_Colliders[index]->IsEnabled()
			|| !ContainsLayer((Layers)m_Colliders[index]->m_Layer, layers)) 
		{
			continue;
		}

		utils::HitInfo hitInfo;
		if (utils::Raycast(m_Colliders[index]->GetPolygon(), orgin, end, hitInfo)) 
		{

			info.push_back(hitInfo);
			colliders.push_back(m_Colliders[index]);
		}
	}
	return hit;
}

bool PhysicsEngine::BoxCast(const Rectf& rect, Collider** collider, const std::vector<Layers> layers, Collider* self)
{
	for (int index{ 0 }; index < m_Colliders.size(); ++index) 
	{
		if (m_Colliders[index] == nullptr
			|| self == m_Colliders[index] 
			|| !m_Colliders[index]->IsEnabled() 
			|| !ContainsLayer((Layers)m_Colliders[index]->m_Layer,layers)) 
		{
			continue;
		}

		if (m_Colliders[index]->CheckCollision(rect)) 
		{
			*collider = m_Colliders[index];
			return true;
		}
	}
	return false;
}

std::vector<Collider*> PhysicsEngine::BoxCastAll(const Rectf& rect, const std::vector<Layers> layers, Collider* self)
{
	std::vector<Collider*> hitColliders;
	for (int index{ 0 }; index < m_Colliders.size(); ++index) 
	{
		if (m_Colliders[index] == nullptr 
			|| self == m_Colliders[index] 
			|| !m_Colliders[index]->IsEnabled() 
			|| !ContainsLayer((Layers)m_Colliders[index]->m_Layer, layers)) 
		{
			continue;
		}

		if (m_Colliders[index]->CheckCollision(rect)) 
		{
			hitColliders.push_back( m_Colliders[index]);
		}
	}
	return hitColliders;
}

bool PhysicsEngine::CircleCast(const Circlef& circle, Collider** collider, const std::vector<Layers> layers, Collider* self)
{
	for (int index{ 0 }; index < m_Colliders.size(); ++index) 
	{
		if (m_Colliders[index] == nullptr 
			|| self == m_Colliders[index] 
			|| !m_Colliders[index]->IsEnabled() 
			|| !ContainsLayer((Layers)m_Colliders[index]->m_Layer, layers)) {
			continue;
		}

		if (m_Colliders[index]->CheckCollision(circle)) 
		{
			*collider = m_Colliders[index];
			return true;
		}
	}
	return false;
}

std::vector<Collider*> PhysicsEngine::CircleCastAll(const Circlef& circle, const std::vector<Layers> layers, Collider* self)
{
	std::vector<Collider*> hitColliders;
	for (int index{ 0 }; index < m_Colliders.size(); ++index) {
		if (m_Colliders[index] == nullptr 
			|| self == m_Colliders[index] 
			|| !m_Colliders[index]->IsEnabled() 
			|| !ContainsLayer((Layers)m_Colliders[index]->m_Layer, layers)) 
		{
			continue;
		}

		if (m_Colliders[index]->CheckCollision(circle)) {
			hitColliders.push_back(m_Colliders[index]);
		}
	}
	return hitColliders;
}

bool PhysicsEngine::ContainsLayer(Layers layer, std::vector<Layers> layers)
{
	for (int index{ 0 }; index < layers.size(); ++index) {
		if (layers[index] == Layers::World) {
			return true;
		}

		if (layer == layers[index]) {
			return true;
		}
	}
	return false;
}



void PhysicsEngine::Draw()
{
	for (int index{ 0 }; index < m_Colliders.size(); ++index)
	{
		if (m_Colliders[index] != nullptr) {
			m_Colliders[index]->Draw();
		}
	}

	for (int index{ 0 }; index < m_Bodies.size(); ++index) {
		if (m_Bodies[index] != nullptr) {
			m_Bodies[index]->Draw();
		}
	}
}

void PhysicsEngine::Deload()
{
	m_Colliders.clear();
}
