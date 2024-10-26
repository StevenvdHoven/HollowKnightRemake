#include "pch.h"
#include "PlayerAnimationHandler.h"
#include "SpriteRenderer.h"
#include "PhyicsBody.h"
#include "PhysicsEngine.h"
#include <iostream>
#include "RectCollider.h"
#include "TextureManager.h"
#include "AudioManager.h"

PlayerAnimationHandler::PlayerAnimationHandler():
	m_FrameWidth{ 2048 / 12 },
	m_FrameHeight{ 2048 / 12 },
	m_FrameRate{ 1.f / 20.f },
	m_Timer{ 0 },
	m_Frame{ 0 },
	m_StartFrame{ 0 },
	m_EndFrame{ 0 },
	m_IsMoving{ false },
	m_IsGrounded{ false },
	m_IsAttacking{ false },
	m_Loop{ false },
	m_State{ PlayerAnimationState::Idle },
	m_Renderer{ nullptr },
	m_Body{ nullptr }
{
}

void PlayerAnimationHandler::Start()
{
	m_Renderer = GetComponent<SpriteRenderer>();
	m_Renderer->SetTexture(GetTextureManager()->GetTexture("Characters/Player.png"));

	UpdateAnimation();
	m_Renderer->SetSource(GetSourceFrame(m_Frame));

	m_Body = GetComponent<PhyicsBody>();

	m_SoundEffects = std::map<PlayerAnimationState, SoundEffect*>
	{
		{Idle, nullptr},
		{Moving, GetAudioManager()->GetAudioClip("Player/hero_run_footsteps_stone.wav")},
		{AirBornUp, nullptr},
		{AirBornDown, GetAudioManager()->GetAudioClip("Player/hero_falling.wav")},
		{Attacking, nullptr},
	};
}

void PlayerAnimationHandler::Update(float elpasedSec)
{
	CheckAnimation();

	if (m_Timer >= m_FrameRate) {
		m_Timer = 0;

		++m_Frame;
		if (m_Frame > m_EndFrame ) {
			if (m_Loop) {
				m_Frame = m_StartFrame;
			}
			else {
				--m_Frame;
			}
		}
		
		m_Renderer->SetSource(GetSourceFrame(m_Frame));
	}

	m_Timer += elpasedSec;
}

void PlayerAnimationHandler::UpdateAnimation()
{
	PlayerAnimationState state{ PlayerAnimationState::Idle };
	
	if (m_IsMoving) state = PlayerAnimationState::Moving;
	if (m_IsGrounded) state = m_Body->m_Velocity.y > 0 ? PlayerAnimationState::AirBornUp : PlayerAnimationState::AirBornDown;
	if (m_IsAttacking) state = PlayerAnimationState::Attacking;

	switch (state)
	{
	case Idle:
		m_StartFrame = 0;
		m_EndFrame = 0;
		m_Loop = true;
		break;
	case Moving:
		m_StartFrame = 1;
		m_EndFrame = 8;
		m_Loop = true;
		break;
	case AirBornUp:
		m_StartFrame = 97;
		m_EndFrame = 101;
		m_Loop = false;
		break;
	case AirBornDown:
		m_StartFrame = 116;
		m_EndFrame = 119;
		m_Loop = true;
		break;
	case Attacking:
		m_StartFrame = 48;
		m_EndFrame = 53;
		m_Loop = false;
		break;
	}

	if (m_State != state) {
		SoundEffect* effect{ m_SoundEffects[m_State] };
		if (effect != nullptr)
		{
			effect->Stop();
		}

		m_Frame = m_StartFrame;
		m_Timer = 0;
		m_State = state;

		effect = m_SoundEffects[m_State];
		if (effect != nullptr)
		{
			effect->Play(0);
		}
	}

	
}

void PlayerAnimationHandler::SetMoving(bool moving)
{
	m_IsMoving = moving;
	UpdateAnimation();
}

void PlayerAnimationHandler::SetGrounded(bool grounded)
{
	m_IsGrounded = grounded;
	UpdateAnimation();
}

void PlayerAnimationHandler::SetAttacking(bool attacking)
{
	m_IsAttacking = attacking;
	UpdateAnimation();
}

Rectf PlayerAnimationHandler::GetSourceFrame(int frame)
{
	
	int index{ 0 };
	for (int row{ 0 }; row < 12; ++row) 
	{
		for (int collum{ 0 }; collum < 12; ++collum) 
		{	
			if (index == frame) {
				return Rectf
				{
					collum * m_FrameWidth,
					row * m_FrameHeight + m_FrameHeight,
					m_FrameWidth,
					m_FrameHeight
				};
			}
			++index;
		}
	}
	return Rectf{};
}

void PlayerAnimationHandler::CheckAnimation()
{
	SetMoving(m_Body->m_Velocity.x != 0);
	Collider* collider{ GetComponent<Collider>() };
	RectCollider* rectCollider{ (RectCollider*)collider };
	Collider* result{ nullptr };
	const std::vector<Layers> layers = { Layers::World };

	Point2f pos{ GetGameObject()->GetPosition() };

	Rectf rect{ rectCollider->GetRect() };
	rect.left = pos.x - rect.width * 0.5f;
	rect.bottom = (pos.y - rect.height * 0.5f) - 50;
	rect.height = 50;

	bool grounded{ GetPhysicsEngine()->BoxCast(rect, &result, layers, rectCollider) };
	
	SetGrounded(!grounded);
}
