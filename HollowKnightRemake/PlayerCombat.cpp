#include "pch.h"
#include "PlayerCombat.h"
#include "PhysicsEngine.h"
#include "PlayerAnimationHandler.h"
#include "TextureManager.h"
#include "ParticleSystem.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "Enemy.h"
#include "Damageable.h"
#include "AudioManager.h"
#include "PlayerMovement.h"
#include "Damageable.h"
#include "PhyicsBody.h"
#include <iostream>

PlayerCombat::PlayerCombat(int damage, float radius, float attackSpeed):
	m_Damage{ damage },
	m_Radius{ radius },
	m_AttackSpeed{ attackSpeed },
	m_AttackTimer{ 0 },
	m_Attacking{ false },
	m_AttackAnimationTime{ 0 },
	m_LookDirection{},
	m_AnimationHandler{nullptr},
	m_PlayerMovement{nullptr},
	m_SlashParticleSystem{nullptr},
	m_ParticleSystem{nullptr},
	m_AttackSound{ nullptr }
{
}


void PlayerCombat::Start()
{
	m_Body = GetComponent<PhyicsBody>();
	m_AnimationHandler = GetComponent<PlayerAnimationHandler>();
	m_PlayerMovement = GetComponent<PlayerMovement>();
	m_Damageable = GetComponent<Damageable>();

	m_SlashParticleSystem = new ParticleSystem
	{ 
		GetTextureManager()->GetTexture("VFX/VFX_KnightSlash.png"),
		5,
		1,
		5,
		0.3f,
		.05f,
		false,
		false 
	};

	std::function<void(Particle&)> onSlashSpawn
	{
		[this](Particle& particle)
		{
			particle.Rotation = float(atan2f(m_LookDirection.y,m_LookDirection.x) * (180.f/M_PI));
		}
	};

	m_SlashParticleSystem->SetSpawnRule(onSlashSpawn);

	m_ParticleSystem = new GameObject();
	m_ParticleSystem->AddComponent(m_SlashParticleSystem);
	GetGameObjectManager()->AddObject(m_ParticleSystem);

	

	m_AttackSound = GetAudioManager()->GetAudioClip("Player/sword1.mp3");
	m_HitSound = GetAudioManager()->GetAudioClip("Player/hero_damage.wav");
	
	CreateHitParticles();
}

void PlayerCombat::Update(float elpasedSec)
{
	m_ParticleSystem->SetPosition(GetGameObject()->GetPosition(), true);
	m_HitParticleSystem->SetPosition(GetGameObject()->GetPosition(), true);

	if (m_Attacking) {
		if (m_AttackTimer >= m_AttackSpeed)
		{
			m_AttackTimer = 0;
			Attack();
		}
	}

	m_AttackTimer += elpasedSec;

	m_AnimationHandler->SetAttacking(m_AttackAnimationTime > 0);
	m_AttackAnimationTime -= elpasedSec;
}

void PlayerCombat::Draw() const
{
	GameObject* owner{ GetGameObject() };
	Point2f position{ owner->GetPosition() };

	Vector2f dir{ m_LookDirection / m_LookDirection.Length() };
	if (m_LookDirection == Vector2f{})
	{
		dir = Vector2f{};
	}
	Circlef circle{ position + dir * (15 + m_Radius),m_Radius };

	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::DrawEllipse(circle.center, circle.radius, circle.radius);
}

void PlayerCombat::KeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_d && m_LookDirection.x == 0)
	{
		m_LookDirection.x = 1;
	}
	else if (e.keysym.sym == SDLK_a && m_LookDirection.x == 0)
	{
		m_LookDirection.x = -1;
	}
	else if (e.keysym.sym == SDLK_w && m_LookDirection.y == 0)
	{
		m_LookDirection.y = 1;
	}
	else if (e.keysym.sym == SDLK_s && m_LookDirection.y == 0)
	{
		m_LookDirection.y = -1;
	}
}

void PlayerCombat::KeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_d && m_LookDirection.x == 1)
	{
		m_LookDirection.x = 0;
	}
	else if (e.keysym.sym == SDLK_a && m_LookDirection.x == -1)
	{
		m_LookDirection.x = -0;
	}
	else if (e.keysym.sym == SDLK_w && m_LookDirection.y == 1)
	{
		m_LookDirection.y = 0;
	}
	else if (e.keysym.sym == SDLK_s && m_LookDirection.y == -1)
	{
		m_LookDirection.y = 0;
	}
}

void PlayerCombat::MouseDownEvent(const SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_LEFT) {
		m_Attacking = true;
	}
	
}
void PlayerCombat::MouseUpEvent(const SDL_MouseButtonEvent& e)
{
	if (e.button == SDL_BUTTON_LEFT) {
		m_Attacking = false;
	}
}

void PlayerCombat::Hit(float stunTime)
{
	if (m_PlayerMovement->IsStunned())
	{
		return;
	}

	m_SplashEffectParent->SetPosition(GetGameObject()->GetPosition(), true);
	m_SplashEffect->Play();
	
	m_HitParticleSystemEffect->Play();
	
	m_HitSound->Play(0);
	

	m_Damageable->TakeDamage(1);
	m_PlayerMovement->Stun(stunTime);

}



void PlayerCombat::Attack()
{
	PhysicsEngine* engine{ GetPhysicsEngine() };
	GameObject* owner{ GetGameObject() };
	Point2f position{ owner->GetPosition() };
	
	Vector2f dir{ m_LookDirection / m_LookDirection.Length() };
	if (m_LookDirection == Vector2f{})
	{
		dir = Vector2f{};
	}

	Circlef circle{ position + dir * (15 + m_Radius),m_Radius };
	
	Collider* self{ GetComponent<Collider>() };
	const std::vector<Layers> layers = { Layers::Entity, Layers::NonEntity };

	std::vector<Collider*> hitColliders{ engine->CircleCastAll(circle, layers, self) };
	for (int index{ 0 }; index < hitColliders.size(); ++index)
	{
		Damageable* damageable{ hitColliders[index]->GetComponent<Damageable>()};
		if (damageable != nullptr)
		{
			damageable->TakeDamage(1);

			if (damageable->GetGameObject()->m_Tag == "enemy" && m_LookDirection.y < 0)
			{
				Vector2f dir{ position - damageable->GetGameObject()->GetPosition() };
				dir = dir / dir.Length();

				m_Body->AddForce(dir * 700, true);
			}
		}

		
	}

	m_SlashParticleSystem->Play();
	m_AttackSound->Play(0);
	m_AttackAnimationTime = .3f;
}

void PlayerCombat::CreateHitParticles()
{
	m_HitParticleSystemEffect = new ParticleSystem
	{
		GetTextureManager()->GetTexture("VFX/PlayerHit.png"),
		1,
		7,
		7,
		.7f,
		.1f,
		false,
		false
	};

	m_HitParticleSystem = new GameObject();
	m_HitParticleSystem->AddComponent(m_HitParticleSystemEffect);
	GetGameObjectManager()->AddObject(m_HitParticleSystem);

	m_SplashEffect = new ParticleSystem
	{
			GetTextureManager()->GetTexture("VFX/black_soul_orb.png"),
			1,
			1,
			1,
			20,
			20,
			false,
	};
	

	ParticleSpawnRules spawnRules
	{
		20,
		15,
		25,
		true
	};

	std::function<void(Particle& particle)> onSpawnRules
	{
		[this](Particle& particle)
		{
			int randAngle{rand() % 361};
			Vector2f velocity{cosf(randAngle) * 1500, sinf(randAngle) * 50 + 50};

			particle.Velocity = velocity;
			particle.Rotation = float(rand() % 361);
			particle.RotationVelocity = int(rand() % 3 - 1) * 80;

			particle.Animate = false;
			particle.LifeTime = 2;
			particle.SpeedOverTime = Curve{ Vector2f{0,1},Vector2f{0,0},Vector2f{0,0},Vector2f{1,0} };
			particle.OpacityOverTime = curve_slope_down;

			particle.TargetGravity = Vector2f{ 0,9.81 };
			particle.GravityOverTime = Curve{ Vector2f{0,0},Vector2f{0,1},Vector2f{-0.1,1},Vector2f{1,1} };
			particle.Mass = 10;

			particle.Scale = float(5.f + rand() % 6) * 0.1f;
		}
	};

	m_SplashEffectParent = new GameObject();
	m_SplashEffectParent->AddComponent(m_SplashEffect);

	m_SplashEffect->SetSpawnRule(spawnRules);
	m_SplashEffect->SetSpawnRule(onSpawnRules);

	GetGameObjectManager()->AddObject(m_SplashEffectParent);
}

