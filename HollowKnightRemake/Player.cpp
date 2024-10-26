#include "pch.h"
#include "Player.h"
#include "PhyicsBody.h"
#include "Collider.h"
#include "RectCollider.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "PlayerMovement.h"
#include "CameraFollower.h"
#include "PlayerAnimationHandler.h"
#include "PhysicsEngine.h"
#include "PlayerCombat.h"
#include "Damageable.h"
#include "PlayerStats.h"

Player::Player()
{
	m_Name = "Player";
	m_Tag = "player";

	PhyicsBody* body{ new PhyicsBody(player_normal_mass,0,false) };
	Collider* collider{ new RectCollider(45,95,{}) };
	collider->m_Layer = Entity;

	SpriteRenderer* renderer{
		new SpriteRenderer(nullptr,
		Rectf{},
		Rectf{}
	) };

	PlayerMovement* movement{ new PlayerMovement(6,700) };
	CameraFollower* cameraFollow{ new CameraFollower(1000,this) };
	PlayerAnimationHandler* animationHanlder{ new PlayerAnimationHandler() };
	PlayerCombat* combatHandler{ new PlayerCombat(1,120,.4f) };
	Damageable* damageable{ new Damageable(5) };
	PlayerStats* playerStats{ new PlayerStats() };


	AddComponent(body);
	AddComponent(movement);
	AddComponent(renderer);
	AddComponent(collider);
	AddComponent(cameraFollow);
	AddComponent(animationHanlder);
	AddComponent(combatHandler);
	AddComponent(damageable);
	AddComponent(playerStats);
	
	SetPosition(Point2f(500, 500));

	m_Transform.Scale = Vector2f(.75f, 0.75f);
}

void Player::StopInput()
{
	GetComponent<PlayerMovement>()->SetEnabled(false);
	GetComponent<PlayerCombat>()->SetEnabled(false);
}

void Player::ResumeInput()
{
	GetComponent<PlayerMovement>()->SetEnabled(true);
	GetComponent<PlayerCombat>()->SetEnabled(true);
}
