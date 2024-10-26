#include "pch.h"
#include "Enemy06.h"
#include "Collider.h"
#include "PhyicsBody.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "PhysicsEngine.h"
#include "PlayerCombat.h"
#include "ParticleSystem.h";
#include "GameObjectManager.h"
#include "Collider.h"
#include "RectCollider.h"
#include <functional>

Enemy06::Enemy06() :
	Enemy(-1, true),
	m_IsUp{ false },
	m_CooldownTimer{ Enemy_06_cooldowntime },
	m_TransitionTimer{ -1 }
{
}

Enemy06::~Enemy06()
{
}

void Enemy06::Start()
{
	Enemy::Start();

	Texture* texture{ GetTextureManager()->GetTexture("Enemies/Enemy_06.png") };
	SpriteRenderer* renderer{ GetComponent<SpriteRenderer>() };
	renderer->SetTexture(texture);

	const float playRate{ 1.f / 10.f };

	Rectf source{ 0,0,texture->GetWidth() / 4, texture->GetHeight() / 2 };
	std::map<int, AnimationSet> animations
	{
		{
			Hidden,
			AnimationSet
			{
				Hidden,
				source,
				0,
				3,
				3,
				playRate,
				true,
				false

			}
		},
		{
			Idle,
			AnimationSet
			{
				Idle,
				source,
				0,
				0,
				3,
				playRate,
				true,
				false

			}
		},
		{
			GoingUp,
			AnimationSet
			{
				GoingUp,
				source,
				1,
				0,
				4,
				1.f / 20.f,
				false,
				false
			}
		},
		{
			GoingDown,
			AnimationSet
			{
				GoingDown,
				source,
				1,
				0,
				4,
				1.f / 20.f,
				false,
				true
			}
		},
	};

	LoadAnimation(renderer, animations);
	SetAnimationState(Hidden);

	GameObject* parent{ GetGameObject() };

	Vector2f up{ parent->GetUpVector() };
	Vector2f right{ parent->GetFrontVector() };

	m_OverlapRect = std::vector<Point2f>
	{
		Point2f(parent->m_Transform.Position + right * Enemy_06_width * 0.5f + -up * Enemy_06_height * 0.5f),
		Point2f(parent->m_Transform.Position + -right * Enemy_06_width * 0.5f + -up * Enemy_06_height * 0.5f),
		Point2f(parent->m_Transform.Position + -right * Enemy_06_width * 0.5f + up * Enemy_06_height * 0.5f),
		Point2f(parent->m_Transform.Position + right * Enemy_06_width * 0.5f + up * Enemy_06_height * 0.5f),
	};

	LoadParticleEffects();
}

void Enemy06::Update(float elpasedSec)
{
	Enemy::Update(elpasedSec);

	if (m_CooldownTimer > 0)
	{
		m_CooldownTimer -= elpasedSec;
		if (m_IsUp) SetAnimationState(Idle);
		else SetAnimationState(Hidden);

		if (m_CooldownTimer <= 0)
		{
			m_TransitionTimer = Enemy_06_transitiontime;

			if (!m_IsUp)
			{
				m_ExplodeRockPuddleEffect->Play();
				m_RockPuddleEffect->Kill();
			}

			m_IsUp = !m_IsUp;
		}
	}

	if (m_TransitionTimer > 0)
	{
		if (m_IsUp) SetAnimationState(GoingUp);
		else SetAnimationState(GoingDown);

		m_TransitionTimer -= elpasedSec;
		if (m_TransitionTimer <= 0)
		{
			m_CooldownTimer = Enemy_06_cooldowntime;
		}
	}

	if (m_IsUp) {
		CheckForPlayerDamage();
	}
	else 
	{
		if (!m_RockPuddleEffect->IsPlaying())
		{
			m_RockPuddleEffect->Play();
		}
	}
	
}

void Enemy06::Draw() const
{
	Enemy::Draw();

	if (m_IsUp) utils::SetColor(Color4f{ 0,1,0,1 });
	else utils::SetColor(Color4f{ 1,0,0,1 });

	utils::DrawPolygon(m_OverlapRect);
}

void Enemy06::CheckForPlayerDamage()
{
	GameObject* parent{ GetGameObject() };
	RectCollider* playerCollider{ GetPlayer()->GetComponent<RectCollider>() };

	PhysicsEngine* engine{ GetPhysicsEngine() };
	if (utils::IsOverlapping(m_OverlapRect,playerCollider->GetRect()))
	{
		PlayerCombat* player{ GetPlayer()->GetComponent<PlayerCombat>()};
		if (player != nullptr)
		{
			PhyicsBody* body{ player->GetComponent<PhyicsBody>() };

			Vector2f dir{ player->GetGameObject()->GetPosition() - parent->GetPosition() };
			dir = dir / dir.Length();

			body->AddForce(dir * 400, true);
			player->Hit(1.5f);
		}
	}
}

void Enemy06::LoadParticleEffects()
{
	GameObject* parent{ GetGameObject() };

	Vector2f up{ parent->GetUpVector() };
	Vector2f right{ parent->GetFrontVector() };

	m_RockPuddleEffect = new ParticleSystem(GetTextureManager()->GetTexture("VFX/rock_particles.png"), 1, 4, 1, 360, 1, false, true);
	
	ParticleSpawnRules spawnRules
	{
		0.1f,
		1,
		1,
		true,
	};

	std::function<void(Particle&)> particleSpawnRule
	{
		[this, up, right](Particle& particle)
		{
			float randAngle{ int(rand() % 361) * (float(M_PI) / 180)};
			Vector2f velocity{cosf(randAngle) * right * 25 + 200 * up};
			randAngle = int(rand() % 361) * (float(M_PI) / 180);
			Point2f pos{ cosf(randAngle) * (Enemy_06_width * 0.25f),0};

			particle.Position = pos;
			particle.Frame = int(rand() % 4);
			particle.Animate = false;
			particle.Velocity = velocity;
			particle.RotationVelocity = (float(rand() % 2) - 1.f) * 180;
			particle.Gravity = Vector2f{ 0,-9.81 };
			particle.Mass = 10;
			particle.LifeTime = 3;
			particle.Scale = (int(rand() % 12 - 10 + 1) + 10) * 0.1f;
		}
	};

	m_RockPuddleEffect->SetSpawnRule(particleSpawnRule);
	m_RockPuddleEffect->SetSpawnRule(spawnRules);

	m_ExplodeRockPuddleEffect = new ParticleSystem(GetTextureManager()->GetTexture("VFX/rock_particles.png"), 1, 4, 1, 360, 1, false, true);
	
	spawnRules = ParticleSpawnRules
	{
		20,
		10,
		15,
		false,
	};
	
	particleSpawnRule = std::function<void(Particle&)>
	{
		[this, up, right](Particle& particle)
		{
			float randAngle{ int(rand() % 361) * (float(M_PI) / 180)};
			Vector2f velocity{cosf(randAngle) * right * 200};
			randAngle = int(rand() % 361) * (float(M_PI) / 180);
			velocity += sinf(randAngle) * up * 100 + up * 300;
			randAngle = int(rand() % 361) * (float(M_PI) / 180);
			Point2f pos{ cosf(randAngle) * (Enemy_06_width * 0.1f),0};

			particle.Position = pos;
			particle.Frame = int(rand() % 4);
			particle.Animate = false;
			particle.Velocity = velocity;
			particle.RotationVelocity = (float(rand() % 2) - 1.f) * 180;
			particle.Gravity = Vector2f{ 0,-9.81 };
			particle.Mass = 10;
			particle.LifeTime = 8;
			particle.Scale = (int(rand() % 20 - 12 + 1) + 12) * 0.1f;
		}
	};

	m_ExplodeRockPuddleEffect->SetSpawnRule(spawnRules);
	m_ExplodeRockPuddleEffect->SetSpawnRule(particleSpawnRule);

	m_RockPuddleVFXGameObject = new GameObject();
	m_RockPuddleVFXGameObject->AddComponent(m_RockPuddleEffect);

	m_ExplodeRockPuddleVFXGameObject = new GameObject();
	m_ExplodeRockPuddleVFXGameObject->AddComponent(m_ExplodeRockPuddleEffect);
	
	GetGameObjectManager()->AddObject(m_RockPuddleVFXGameObject);
	GetGameObjectManager()->AddObject(m_ExplodeRockPuddleVFXGameObject);

	m_RockPuddleVFXGameObject->SetPosition(parent->GetPosition() - up * Enemy_06_height * 0.5f, true);
	m_ExplodeRockPuddleVFXGameObject->SetPosition(parent->GetPosition() - up * Enemy_06_height * 0.5f, true);

	m_RockPuddleEffect->SetOrder(10);
	m_ExplodeRockPuddleEffect->SetOrder(10);
}

