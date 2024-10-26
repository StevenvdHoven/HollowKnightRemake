#include "pch.h"
#include "TikTik.h"
#include "Enemy02.h"
#include "SpriteRenderer.h"
#include "RectCollider.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"

TikTik::TikTik()
{
	m_Name = "TikTik";
	m_Tag = "enemy";

	Enemy02* enemyBehavior{ new Enemy02() };
	Damageable* damageable{ new Damageable(3) };

	RectCollider* collider{ new RectCollider(50,50,{}) };
	collider->m_Layer = Layers::Entity;

	PhyicsBody* body{ new PhyicsBody(60,0,false) };
	body->SetBodyOnBodyCollision(false);
	SpriteRenderer* renderer{ new SpriteRenderer(nullptr,Rectf{},Rectf{}) };

	AddComponent((Enemy*)enemyBehavior);
	AddComponent(damageable);
	AddComponent(collider);
	AddComponent(renderer);
	AddComponent(body);
}
