#include "pch.h"
#include "Enemy03.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"
#include "PlayerCombat.h"
#include "RectCollider.h"
#include "GameObjectManager.h"

Enemy03::Enemy03() :
	Enemy(enemy_03_health),
	m_Direction{},
	m_StunTimer{ -1 },
	m_Dead{ false },
	m_DieTimer{ 0 },
	m_Scale{ 1 },
	m_RecalculateTimer{-1},
	m_Body{ nullptr },
	m_Collider{ nullptr }
{
}

void Enemy03::Start()
{
	Enemy::Start();

	int angle{ rand() % 361 };

	m_Direction = Vector2f
	{
		float(cos(angle)),
		float(sin(angle))
	};

	m_Body = GetComponent<PhyicsBody>();
	m_Collider = GetComponent<Collider>();

	m_Scale = GetGameObject()->m_Transform.Scale.x;


	Texture* texture{ GetTextureManager()->GetTexture("Enemies/Enemy_03.png") };
	SpriteRenderer* renderer{ GetComponent<SpriteRenderer>() };
	renderer->SetTexture(texture);

	const float playRate{ 1.f / 10.f };

	Rectf source{ 0,0,texture->GetWidth() / 4, texture->GetHeight() / 3 };
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
				4,
				playRate,
				true,
				false
			}
		},
		{
			Hit,
			AnimationSet
			{
				Hit,
				source,
				1,
				0,
				2,
				1.f / 2.f,
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
				2,
				0,
				4,
				1.f / 5.f,
				false,
				false
			}
		},
	};

	LoadAnimation(renderer, animations);
}

void Enemy03::Update(float elpasedSec)
{
	Enemy::Update(elpasedSec);

	if (m_Dead)
	{
		SetAnimationState(Dying);
		if (m_DieTimer <= 0)
		{
			GetGameObjectManager()->DeleteObject(GetGameObject());
		}
		else {
			m_DieTimer -= elpasedSec;
		}
		return;
	}

	Move(elpasedSec);

	CheckForPlayerDamage();

	if (m_RecalculateTimer > 0)
	{
		m_RecalculateTimer -= elpasedSec;
	}
}

void Enemy03::Move(float elpasedSec)
{
	int scaler{ m_Body->m_Velocity.x >= 1 ? -1 : 1 };
	GetGameObject()->m_Transform.Scale.x = m_Scale * scaler;

	if (m_StunTimer < 0)
	{
		SetAnimationState(Flying);

		m_Body->AddForce(m_Direction * enemy_03_max_speed, true);

		if (m_RecalculateTimer < 0) 
		{
			Detect();
		}
		
	}
	else
	{
		SetAnimationState(Hit);
		m_StunTimer -= elpasedSec;
		if (m_StunTimer < 0) {
			m_Body->m_Velocity = Vector2f{};
		}
	}
}

void Enemy03::Draw() const
{
	GameObject* parent{ GetGameObject() };
	Point2f end{ parent->GetPosition() + m_Direction * enemy_03_detection_range };

	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::DrawLine(parent->GetPosition(), end);

	const Ellipsef circle{ parent->GetPosition(),enemy_03_damage_range,enemy_03_damage_range };
	utils::DrawEllipse(circle);

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

void Enemy03::CheckForPlayerDamage()
{
	GameObject* parent{ GetGameObject() };
	const Circlef circle{ parent->GetPosition(),enemy_03_damage_range };
	if (isOverlappingWithPlayer(circle))
	{

		PlayerCombat* playerCombat{ GetPlayer()->GetComponent<PlayerCombat>() };

		playerCombat->Hit(1.5f);

		PhyicsBody* playerBody{ GetPlayer()->GetComponent<PhyicsBody>() };
		Vector2f dir{ playerCombat->GetGameObject()->GetPosition() - parent->GetPosition() };
		dir = dir / dir.Length();
		playerBody->AddForce(dir * 200, true);

	}
}

void Enemy03::OnTakeDamage(int damage)
{
	Enemy::OnTakeDamage(damage);
	GameObject* parent{ GetGameObject() };
	Vector2f dir{ GetPlayer()->GetPosition() - parent->GetPosition() };
	dir = dir / dir.Length();
	m_Body->AddForce(-dir * 200, true);

	m_StunTimer = enemy_03_stun_time;
}

void Enemy03::OnDead()
{
	Enemy::OnDead();

	m_Dead = true;
	m_DieTimer = .75f;
}

int Enemy03::GetTinValue()
{
	return enemy_03_tin_value;
}

Collider* Enemy03::Detect()
{
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

	for (int index{ 0 }; index < directions.size(); ++index)
	{
		Point2f start{ GetGameObject()->GetPosition() };
		Point2f end{ start + directions[index] * 50 };

		utils::HitInfo info;

		if (engine->Raycast(start, end, info, &result, layers, m_Collider))
		{
			Vector2f reflection(m_Direction.Reflect(info.normal));
			reflection = reflection / reflection.Length();
			m_Direction = reflection;

			m_RecalculateTimer = 1;
			return result;
		}
	}

	return nullptr;
}

