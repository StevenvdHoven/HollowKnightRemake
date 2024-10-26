#include "pch.h"
#include "Enemy04.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "PhyicsBody.h"
#include "RectCollider.h"
#include "PhysicsEngine.h"
#include "GameObjectManager.h"
#include "ParticleSystem.h"
#include "PlayerCombat.h"
#include "utils.h"

Enemy04::Enemy04():
	Enemy(3),
	m_Direction{},
	m_StunTimer{-1},
	m_Dead{false},
	m_DieTimer{4},
	m_DirectionTimer{-1},
	m_DetectionTimer{-1},
	m_ShootTime{ -1 },
	m_ShootCooldown{5}
{
}

Enemy04::~Enemy04()
{
}

void Enemy04::Start()
{
	Enemy::Start();

	m_Body = GetComponent<PhyicsBody>();
	m_Collider = GetComponent<Collider>();

	Texture* texture{ GetTextureManager()->GetTexture("Enemies/Enemy_04.png") };
	SpriteRenderer* renderer{ GetComponent<SpriteRenderer>() };
	renderer->SetTexture(texture);

	const float playRate{ 1.f / 10.f };

	Rectf source{ 0,0,texture->GetWidth() / 6, texture->GetHeight() / 5 };
	std::map<int, AnimationSet> animations
	{
		{
			Flying,
			AnimationSet
			{
				Flying,
				source,
				0,
				0,
				7,
				playRate,
				true,
				false
			}
		},
		{
			Spit,
			AnimationSet
			{
				Spit,
				source,
				2,
				0,
				6,
				1.f / 10.f,
				false,
				false,
			}
		},
		{
			Spit2,
			AnimationSet
			{
				Spit2,
				source,
				3,
				0,
				6,
				1.f / 10.f,
				false,
				false,
			}
		},
		{
			Dying,
			AnimationSet
			{
				Dying,
				source,
				4,
				0,
				6,
				1.f / 5.f,
				false,
				false
			}
		},
	};

	

	m_ShootParticle = new ParticleSystem
	{
		GetTextureManager()->GetTexture("VFX/SpitShot.png"),
		7,
		1,
		7,
		20,
		0.1f,
		false,
		false
	};

	ParticleSpawnRules spawnRules
	{
		50,
		1,
		1,
		true
	};

	std::function<void(Particle&)> onShootSpawnRule
	{
		[this](Particle& particle)
		{
			Vector2f dir{GetPlayer()->GetPosition() - GetGameObject()->GetPosition()};
			particle.Velocity = (dir / dir.Length()) * 600;
			particle.Rotation = atan2f(dir.y, dir.x) * (180 / M_PI);
			particle.IsCollider = true;
			particle.Radius = 25;
			particle.Frame = 0;
			particle.Animate = false;
			particle.LifeTime = 20;
			particle.Loop = false;
		}
	};

	std::function<void(Particle&, Collider*)> onCollideRule
	{
		[this](Particle& particle, Collider* collider)
		{
			if (collider == m_Collider || collider->m_IsTrigger)
			{
				return;
			}

			particle.Animate = true;
			particle.LifeTime = .5;
			particle.Velocity = Vector2f{};
			particle.IsCollider = false;

			utils::HitInfo info;
			if (utils::Raycast(collider->GetPolygon(), m_ShootParticleParent->GetPosition(), collider->GetPosition(), info))
			{
				particle.Rotation = atan2f(info.normal.y, info.normal.x) * (180 / M_PI) - 90;
			}
			
			if (collider->GetGameObject()->m_Tag == "player")
			{
				PlayerCombat* combat{ collider->GetComponent<PlayerCombat>() };
				combat->Hit();

				PhyicsBody* playerBody{ collider->GetComponent<PhyicsBody>() };
				Vector2f dir{ collider->GetPosition() - (m_ShootParticleParent->GetPosition() + Vector2f(particle.Position)) };
				dir = dir / dir.Length();

				playerBody->AddForce(dir * 200, true);
			}
		}
	};

	m_ShootParticle->SetSpawnRule(spawnRules);
	m_ShootParticle->SetSpawnRule(onShootSpawnRule);
	m_ShootParticle->SetOnCollideRule(onCollideRule);

	m_ShootParticleParent = new GameObject();
	m_ShootParticleParent->AddComponent(m_ShootParticle);

	GetGameObjectManager()->AddObject(m_ShootParticleParent);


	LoadAnimation(renderer, animations);
}

void Enemy04::Update(float elpasedSec)
{
	Enemy::Update(elpasedSec);

	if (m_Dead)
	{
		SetAnimationState(Dying);
		if (m_DieTimer < 0)
		{
			GetComponent<SpriteRenderer>()->SetEnabled(false);
			GetComponent<Collider>()->SetEnabled(false);
		}

		if (m_DieTimer <= -10)
		{
			GetGameObjectManager()->DeleteObject(GetGameObject());
		}
		else {
			m_DieTimer -= elpasedSec;
		}
		return;
	}

	if (m_DetectionTimer > 0)
	{
		m_DetectionTimer -= elpasedSec;
	}

	if (SeesPlayer())
	{
		if (m_ShootCooldown > 0)
		{
			m_ShootCooldown -= elpasedSec;
			if (m_ShootCooldown <= 0)
			{
				m_ShootTime = enemy_04_shoot_time;
			}
		}
		
		if (m_ShootTime > 0) 
		{
			m_ShootTime -= elpasedSec;
			if (m_ShootTime < .2f && m_ShootCooldown < 0)
			{
				Shoot();
			}
			else if (m_ShootTime < 0.5f)
			{
				SetAnimationState(Spit2);
			}
			else if (m_ShootCooldown > 0.5f)
			{
				SetAnimationState(Spit);
				m_Body->m_Velocity = Vector2f{};
			}
			return;
		}


		CheckForDirection();
		Move(elpasedSec);
	}
	else
	{
		SetAnimationState(Flying);
	}

}

void Enemy04::Move(float elpasedSec)
{
	int scaler{ m_Body->m_Velocity.x >= 1 ? -1 : 1 };
	GetGameObject()->m_Transform.Scale.x = abs(GetGameObject()->m_Transform.Scale.x) * scaler;

	if (m_StunTimer < 0)
	{
		SetAnimationState(Flying);
		m_Body->AddForce(m_Direction * enemy_04_max_speed, true);
	}
}

void Enemy04::Draw() const
{
	Enemy::Draw();

	Ellipsef ellipse{ GetGameObject()->GetPosition(),enemy_04_detection_range,enemy_04_detection_range };
	utils::SetColor(Color4f{0, 1, 0, 1});
	utils::DrawEllipse(ellipse);

	ellipse = Ellipsef{ GetGameObject()->GetPosition(),enemy_04_lose_range,enemy_04_lose_range };
	utils::SetColor(Color4f{ 0,0,1,1 });
	utils::DrawEllipse(ellipse);

	ellipse = Ellipsef{ GetGameObject()->GetPosition(),enemy_04_shoot_range,enemy_04_shoot_range };
	utils::SetColor(Color4f{ 1.f,.6f,0,1 });
	utils::DrawEllipse(ellipse);

	std::vector<Vector2f> directions
	{
		Vector2f{0,1},
		Vector2f{1,1},
		Vector2f{1,0},
		Vector2f{1,-1},
		Vector2f{0,-1},
		Vector2f{-1,-1},
		Vector2f{-1,0},
		Vector2f{-1,1},
	};

	utils::SetColor(Color4f{ 0,0,1,1 });

	for (int index{ 0 }; index < directions.size(); ++index)
	{
		Point2f start{ GetGameObject()->GetPosition() };
		Point2f end{ start + directions[index] * 50 };
		utils::DrawLine(start, end);
	}
}

void Enemy04::CheckForPlayerDamage()
{
}

void Enemy04::OnTakeDamage(int damage)
{
	Enemy::OnTakeDamage(damage);

	GameObject* parent{ GetGameObject() };
	Vector2f dir{ GetPlayer()->GetPosition() - parent->GetPosition() };
	dir = dir / dir.Length();
	m_Body->AddForce(-dir * 200, true);

	m_StunTimer = enemy_04_stun_time;
}

void Enemy04::OnDead()
{
	Enemy::OnDead();
	
	m_Dead = true;
	m_DieTimer = .75f;
}

int Enemy04::GetTinValue()
{
	return enemy_04_tin_value;
}

bool Enemy04::SeesPlayer()
{
	const float distance{ utils::GetDistance(GetPlayer()->GetPosition(),GetGameObject()->GetPosition()) };

	if (m_SeesPlayer) 
	{
		m_SeesPlayer = !(distance > enemy_04_lose_range);
	}
	else 
	{
		m_SeesPlayer = distance < enemy_04_detection_range;
	}

	return m_SeesPlayer;
}

void Enemy04::CheckForDirection()
{
	if (m_DetectionTimer > 0)
	{
		return;
	}

	GameObject* parent{ GetGameObject() };

	Collider* result = nullptr;

	Point2f start{ parent->GetPosition() };

	std::vector<Vector2f> directions
	{
		Vector2f{0,1},
		Vector2f{1,1},
		Vector2f{1,0},
		Vector2f{1,-1},
		Vector2f{0,-1},
		Vector2f{-1,-1},
		Vector2f{-1,0},
		Vector2f{-1,1},
	};

	PhysicsEngine* engine{ GetPhysicsEngine() };
	const std::vector<Layers> layers{ Layers::Static };

	Vector2f combinedForce{ 0,0 };

	for (int index{ 0 }; index < directions.size(); ++index)
	{
		Point2f start{ GetGameObject()->GetPosition() };
		Point2f end{ start + directions[index] * 50 };

		utils::HitInfo info;

		if (engine->Raycast(start, end, info, &result, layers, m_Collider))
		{
			combinedForce += -directions[index];
		}
	}

	if (combinedForce != Vector2f{ 0,0 })
	{
		combinedForce = combinedForce / combinedForce.Length();
	}
	

	if (m_SeesPlayer) 
	{
		const float distance{ utils::GetDistance(GetPlayer()->GetPosition(),GetGameObject()->GetPosition()) };
		Vector2f dir{ GetPlayer()->GetPosition() - GetGameObject()->GetPosition() };
		dir = dir / dir.Length();

		Vector2f caculatedForce{ dir * 0.6f + combinedForce * 0.4f };
		if (distance > enemy_04_shoot_range)
		{
			m_Direction = caculatedForce;
			
		}
		else 
		{
			m_Direction = -caculatedForce;
		}
		m_DetectionTimer = 1;
		return;
	}

	

	

	if (m_DirectionTimer < 0)
	{
		CaculateRandomDiretion();
		m_DirectionTimer = 1;
	}

}

void Enemy04::CaculateRandomDiretion()
{
	int randAngle{ rand() % 361 };
	Vector2f dir{ cosf(randAngle), sinf(randAngle) };
	m_Direction = dir;

	m_DetectionTimer = 1;
}

void Enemy04::Shoot()
{
	if (m_ShootCooldown > 0)
	{
		return;
	}

	m_ShootParticleParent->SetPosition(GetGameObject()->GetPosition());
	m_ShootParticle->Play();

	m_ShootCooldown = enemy_04_shoot_cooldown;
	
}
