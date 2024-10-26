#include "pch.h"
#include "Goam.h"
#include "Enemy06.h"
#include "SpriteRenderer.h"
#include "RectCollider.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"

Goam::Goam()
{
	m_Name = "Goam";
	m_Tag = "enemy";

	Enemy06* enemyBehavior{ new Enemy06() };
	Damageable* damageable{ new Damageable(3) };

	SpriteRenderer* renderer{ new SpriteRenderer(nullptr,Rectf{},Rectf{}) };

	AddComponent((Enemy*)enemyBehavior);
	AddComponent(damageable);
	AddComponent(renderer);
}
