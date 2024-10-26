#include "pch.h"
#include "HuskBully.h"
#include "SpriteRenderer.h"
#include "RectCollider.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"
#include "Damageable.h"
#include "Enemy05.h"

HuskBully::HuskBully()
{
	m_Name = "HuskBully";
	m_Tag = "enemy";

	Enemy05* enemyBehavior{ new Enemy05() };
	Damageable* damageable{ new Damageable(3) };

	RectCollider* collider{ new RectCollider(60,80,{}) };
	collider->m_Layer = Layers::Entity;

	PhyicsBody* body{ new PhyicsBody(60,0,false) };
	body->SetBodyOnBodyCollision(false);
	SpriteRenderer* renderer{ new SpriteRenderer(nullptr,Rectf{},Rectf{}) };

	AddComponent((Enemy*)enemyBehavior);
	AddComponent(damageable);
	AddComponent(collider);
	AddComponent(body);
	AddComponent(renderer);

	m_Transform.Scale = Vector2f{ .75f,.75f };
}
