#include "pch.h"
#include "Vengefly.h"
#include "SpriteRenderer.h"
#include "RectCollider.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"
#include "Damageable.h"
#include "Enemy07.h"

Vengefly::Vengefly()
{
	m_Name = "Vengefly";
	m_Tag = "enemy";

	Enemy07* enemyBehavior{ new Enemy07() };
	Damageable* damageable{ new Damageable(3) };

	RectCollider* collider{ new RectCollider(80,50,{}) };
	collider->m_Layer = Layers::Entity;

	PhyicsBody* body{ new PhyicsBody(60,0,false) };
	body->SetBodyOnBodyCollision(false);
	body->m_gravity = 0;
	SpriteRenderer* renderer{ new SpriteRenderer(nullptr,Rectf{},Rectf{}) };

	AddComponent((Enemy*)enemyBehavior);
	AddComponent(damageable);
	AddComponent(collider);
	AddComponent(body);
	AddComponent(renderer);
}
