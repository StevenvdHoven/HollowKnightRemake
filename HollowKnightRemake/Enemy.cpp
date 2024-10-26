#include "pch.h"
#include "Enemy.h"
#include "GameObjectManager.h"
#include "SpriteRenderer.h"
#include "Collider.h"
#include "utils.h"
#include "AudioManager.h"
#include "PlayerStats.h"

Enemy::Enemy(int health, bool useAnimation) :
	m_CurrentState{0},
	m_CurrentFrame{0},
	m_AnimationTimer{0},
	m_UseAnimation{ useAnimation },
	m_Renderer{nullptr},
	m_PlayerGameObject{ nullptr },
	m_DamageAble{nullptr}
{
}

Enemy::~Enemy()
{
	GetGameObjectManager()->DeleteObject(m_SplashEffect);
}

void Enemy::Start()
{
	
	m_DamageAble = GetComponent<Damageable>();
	m_DamageAble->AddListenerTakeDamage([this](int damage) {OnTakeDamage(damage); });
	m_DamageAble->AddListenerOnDead([this](){OnDead();});

	m_PlayerGameObject = GetGameObjectManager()->GetObjectBasedOfTag("player");
	m_PlayerCollider = m_PlayerGameObject->GetComponent<Collider>();

	m_SplashEffect = new EnemySplashParticleEffect();
	GetGameObjectManager()->AddObject(m_SplashEffect);

	m_CoinParticleObject = new CoinParticleObject();
	GetGameObjectManager()->AddObject(m_CoinParticleObject);

	m_OnHitSound = GetAudioManager()->GetAudioClip("SFX/Splat 01.mp3");
	m_OnDeadSound = GetAudioManager()->GetAudioClip("SFX/explosion 4 wet original.mp3");
}

void Enemy::Update(float elpasedSec)
{
	UpdateAnimation(elpasedSec);
}

void Enemy::Draw() const
{
}



void Enemy::OnTakeDamage(int damage)
{
	if (m_DamageAble->GetHealth() <= 0)
	{
		return;
	}

	

	m_SplashEffect->Play(GetGameObject()->GetPosition());
	m_OnHitSound->Play(0);
}

void Enemy::OnDead()
{
	GetPlayer()->GetComponent<PlayerStats>()->GiveEnergy(11.f);

	m_OnDeadSound->SetVolume(40);
	m_OnDeadSound->Play(0);
	m_CoinParticleObject->Play(GetGameObject()->GetPosition(),GetTinValue(), GetSilverValue());
}

void Enemy::LoadAnimation(SpriteRenderer* renderer, std::map<int, AnimationSet> animations)
{
	m_Renderer = renderer;
	m_AllAnimations = animations;


}

void Enemy::SetAnimationState(int state)
{
	if (state != m_CurrentState)
	{
		m_CurrentState = state;
		m_AnimationTimer = m_AllAnimations[m_CurrentState].PlaySpeed;
		m_CurrentFrame = !m_AllAnimations[m_CurrentState].reverse ? m_AllAnimations[m_CurrentState].StartFrame : m_AllAnimations[m_CurrentState].MaxFramesOnRow - 1;
	}
}

void Enemy::OnCollisionEnter(Collider* collider)
{
}

void Enemy::OnCollisionStay(Collider* collider)
{
}

void Enemy::OnCollisionExit(Collider* collider)
{
}

GameObject* Enemy::GetPlayer()
{
	return m_PlayerGameObject;
}

int Enemy::GetHealth()
{
	return m_DamageAble->GetHealth();
}

bool Enemy::isOverlappingWithPlayer(const Rectf& rect)
{
	return m_PlayerCollider->CheckCollision(rect);
}

bool Enemy::isOverlappingWithPlayer(const Circlef& circle)
{
	return m_PlayerCollider->CheckCollision(circle);
}

bool Enemy::isOverlappingWithPlayer(Collider* collider) 
{
	return m_PlayerCollider->CheckCollision(collider);
}

int Enemy::GetTinValue()
{
	return 0;
}

int Enemy::GetSilverValue()
{
	return 0;
}

void Enemy::UpdateAnimation(float elapsedSec)
{
	if (!m_UseAnimation)
	{
		return;
	}

	m_AnimationTimer -= elapsedSec;
	if (m_AnimationTimer <= 0) {
		m_AnimationTimer = m_AllAnimations[m_CurrentState].PlaySpeed;

		if (!m_AllAnimations[m_CurrentState].reverse) 
		{
			if (m_CurrentFrame >= m_AllAnimations[m_CurrentState].MaxFramesOnRow - 1)
			{
				if (m_AllAnimations[m_CurrentState].Loop) {
					m_CurrentFrame = m_AllAnimations[m_CurrentState].StartFrame;
				}
				else {
					m_CurrentFrame = m_AllAnimations[m_CurrentState].MaxFramesOnRow - 1;
				}
			}
			else {
				++m_CurrentFrame;
			}
		}
		else 
		{
			if (m_CurrentFrame <= 0)
			{
				if (m_AllAnimations[m_CurrentState].Loop) {
					m_CurrentFrame = m_AllAnimations[m_CurrentState].MaxFramesOnRow - 1;
				}
				else {
					m_CurrentFrame = m_AllAnimations[m_CurrentState].StartFrame;
				}
			}
			else 
			{
				--m_CurrentFrame;
			}
			
		}
		
	}
	m_Renderer->SetSource(GetSource());

}

Rectf Enemy::GetSource()
{
	Rectf source{ m_AllAnimations[m_CurrentState].Source };
	return Rectf
	{
		m_CurrentFrame * source.width,
		m_AllAnimations[m_CurrentState].Row * source.height + source.height,
		source.width,
		source.height
	};
}


