#include "pch.h"
#include "PlayerMovement.h"
#include "PhyicsBody.h"
#include <iostream>
#include "PhysicsEngine.h"
#include "AudioManager.h"
#include "RectCollider.h"

PlayerMovement::PlayerMovement(float speed, float jumpforce) :
	m_Speed{ speed },
	m_JumpForce{ jumpforce },
	m_InputDirectionX{ 0 },
	m_Body{ nullptr },
	m_CanJump{ false },
	m_StunTime{0},
	m_Stunned{false},
	m_Paused{false}
{
}

void PlayerMovement::Start()
{
	m_Body = GetComponent<PhyicsBody>();
	m_JumpSound = GetAudioManager()->GetAudioClip("Player/hero_jump.wav");
}

void PlayerMovement::Update(float elpasedSec)
{
	if (m_Paused)
	{
		return;
	}

	if (m_StunTime <= 0) {
		if (m_Stunned) 
		{
			m_Body->m_Velocity.x = 0;
			m_Stunned = false;
			m_Body->SetBodyOnBodyCollision(true);
		}

		m_Body->m_Velocity.x = m_InputDirectionX * player_normal_mass * m_Speed;
	}
	else {
		m_StunTime -= elpasedSec;
	}

	RectCollider* collider{ GetComponent<RectCollider>() };
	Rectf rect{ collider->GetRect() };
	rect.bottom -= 10;

	Collider* result;
	if (GetPhysicsEngine()->BoxCast(rect, &result, { Static }, collider))
	{
		m_CanJump = true;
	}
	else 
	{
		m_CanJump = false;
	}
	
}

void PlayerMovement::KeyDownEvent(const SDL_KeyboardEvent& e)
{

	if (e.keysym.sym == SDLK_a) {
		m_InputDirectionX = -1;
		GetGameObject()->m_Transform.Scale.x = -.75f;
	}
	if (e.keysym.sym == SDLK_d) {
		m_InputDirectionX = 1;
		GetGameObject()->m_Transform.Scale.x = .75f;
	}

 	if (e.keysym.sym == SDLK_SPACE && m_CanJump) {

		m_Body->AddForce(Vector2f(0, m_JumpForce));
		GetGameObject()->SetPosition(GetGameObject()->GetPosition() + Vector2f(0, 2), true);
		m_CanJump = false;

		m_Body->m_Mass = player_jump_mass;
		m_JumpSound->Play(0);
	}
}

void PlayerMovement::KeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_a) {
		m_InputDirectionX = 0;
	}
	if (e.keysym.sym == SDLK_d) {
		m_InputDirectionX = 0;
	}
	if (e.keysym.sym == SDLK_SPACE)
	{
		m_Body->m_Mass = player_normal_mass;
	}
}

void PlayerMovement::OnCollisionEnter(Collider* collider)
{
	
}

void PlayerMovement::OnCollisionStay(Collider* collider)
{
}

void PlayerMovement::OnCollisionExit(Collider* collider)
{
}

void PlayerMovement::Stun(float time)
{
	m_StunTime = time;
	m_Stunned = true;
	m_Body->SetBodyOnBodyCollision(false);
}

bool PlayerMovement::IsStunned()
{
	return m_Stunned;
}

void PlayerMovement::SetPause(bool pause)
{
	m_Paused = pause;
}
