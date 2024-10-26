#include "pch.h"
#include "Crawlid.h"
#include "Enemy01.h"
#include "SpriteRenderer.h"
#include "RectCollider.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"

Crawlid::Crawlid()
{
	m_Name = "Crawlid";
	m_Tag = "enemy";

	Enemy01* enemyBehavior{ new Enemy01() };
	Damageable* damageable{ new Damageable(3) };

	RectCollider* collider{ new RectCollider(80,50,{})};
	collider->m_Layer = Layers::Entity;

	PhyicsBody* body{ new PhyicsBody(60,0,false) };
	body->SetBodyOnBodyCollision(false);
	SpriteRenderer* renderer{ new SpriteRenderer(nullptr,Rectf{},Rectf{}) };
	
	AddComponent((Enemy*)enemyBehavior);
	AddComponent(damageable);
	AddComponent(collider);
	AddComponent(body);
	AddComponent(renderer);
	
}
