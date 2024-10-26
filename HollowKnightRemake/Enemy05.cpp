#include "pch.h"
#include "Enemy05.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"
#include "PlayerCombat.h"
#include "GameObjectManager.h"

Enemy05::Enemy05() :
	Enemy(enemy_05_health),
	m_TurnTimer{ -1 },
	m_StunTimer{ -1 },
	m_ChargeTimer{ -1 },
	m_DieTimer{ -1 },
	m_Dead{ false }
{
}

void Enemy05::Start()
{
	Enemy::Start();

	m_Collider = GetComponent<Collider>();
	m_Body = GetComponent<PhyicsBody>();


	Texture* texture{ GetTextureManager()->GetTexture("Enemies/Enemy_05.png") };
	SpriteRenderer* renderer{ GetComponent<SpriteRenderer>() };
	renderer->SetTexture(texture);

	const float playRate{ 1.f / 10.f };

	Rectf source{ 0,0,texture->GetWidth() / 7, texture->GetHeight() / 9 };
	Rectf diedSource{ 0,0,texture->GetWidth() / 5,texture->GetHeight() / 9 };
	std::map<int, AnimationSet> animations
	{
		{
			Idle,
			AnimationSet
			{
				Idle,
				source,
				0,
				0,
				6,
				playRate,
				true,
				false

			}
		},
		{
			Walking,
			AnimationSet
			{
				Walking,
				source,
				1,
				0,
				7,
				playRate,
				true,
				false
			}
		},
		{
			Turning,
			AnimationSet
			{
				Turning,
				source,
				2,
				0,
				2,
				1.f / 2.f,
				false,
				false
			}
		},
		{
			PreCharge,
			AnimationSet
			{
				PreCharge,
				source,
				3,
				0,
				4,
				1.f / 10.f,
				false,
				false
			}
		},
		{
			Charging,
			AnimationSet
			{
				Charging,
				source,
				4,
				0,
				2,
				1.f / 5.f,
				false,
				false
			}
		},
		{
			PostCharge,
			AnimationSet
			{
				PostCharge,
				source,
				5,
				0,
				1,
				1.f / 5.f,
				false,
				false
			}
		},
		{
			Dying,
			AnimationSet
			{
				Dying,
				diedSource,
				6,
				0,
				4,
				1.f / 5.f,
				false,
				false
			}
		},
		{
			Died,
			AnimationSet
			{
				Died,
				diedSource,
				7,
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

void Enemy05::Update(float elpasedSec)
{
	Enemy::Update(elpasedSec);

	if (m_Dead)
	{
		m_DieTimer -= elpasedSec;

		if (m_DieTimer < 1)
		{
			SetAnimationState(Died);
			if (m_DieTimer <= 0)
			{
				GetGameObjectManager()->DeleteObject(GetGameObject());
				return;
			}
		}
		else if (m_DieTimer < 1.5f)
		{
			SetAnimationState(Dying);
		}
		return;
	}

	CheckForPlayerDamage();

	if (m_StunTimer > 0 && m_ChargeTimer < 0 && m_TurnTimer < 0)
	{
		m_StunTimer -= elpasedSec;
		return;
	}

	if (m_TurnTimer > 0)
	{
		SetAnimationState(Turning);
		m_TurnTimer -= elpasedSec;
		if (m_TurnTimer <= 0)
		{
			float scale{ GetGameObject()->m_Transform.Scale.x };
			GetGameObject()->m_Transform.Scale.x = -scale;
		}
		else
		{
			return;
		}

	}

	if (CheckForPlayer() || m_ChargeTimer > 0)
	{
		Charge(elpasedSec);
		return;
	}
	else
	{
		if (!Detect())
		{
			Move(elpasedSec);
		}
		else
		{
			m_TurnTimer = 1;
		}
	}
}

void Enemy05::Draw() const
{
	Enemy::Draw();

	GameObject* parent{ GetGameObject() };
	Vector2f forward{ -parent->GetFrontVector() };

	const Ellipsef circle{ parent->GetPosition(),enemy_05_damage_range,enemy_05_damage_range };
	utils::DrawEllipse(circle);

	utils::DrawLine(parent->GetPosition(),parent->GetPosition() + forward * 50);
}

void Enemy05::CheckForPlayerDamage()
{
	GameObject* parent{ GetGameObject() };
	const Circlef circle{ parent->GetPosition(),enemy_05_damage_range };
	if (isOverlappingWithPlayer(circle))
	{

		PlayerCombat* playerCombat{ GetPlayer()->GetComponent<PlayerCombat>() };

		playerCombat->Hit(1.5f);

		PhyicsBody* playerBody{ GetPlayer()->GetComponent<PhyicsBody>() };
		Vector2f dir{ playerCombat->GetGameObject()->GetPosition() - parent->GetPosition() };
		dir = dir / dir.Length();
		playerBody->AddForce(dir * 400, true);

	}
}

void Enemy05::OnTakeDamage(int damage)
{
	Enemy::OnTakeDamage(damage);
	m_StunTimer = 1;
}

void Enemy05::OnDead()
{
	Enemy::OnDead();

	m_DieTimer = 1.5f;
	m_Dead = true;
}

int Enemy05::GetTinValue()
{
	return enemy_05_tin_value;
}


bool Enemy05::Detect()
{
	PhysicsEngine* engine{ GetPhysicsEngine() };

	GameObject* parent{ GetGameObject() };
	Vector2f forward{ -parent->GetFrontVector() };

	Point2f start{ parent->GetPosition() };
	Point2f end{ start + forward * enemy_05_range };

	Collider* resultCollider;
	utils::HitInfo info;

	std::vector<Layers> layer{ Static };

	return engine->Raycast(start, end, info, &resultCollider, layer, m_Collider);
}

bool Enemy05::CheckForPlayer()
{
	Point2f playerPos{ GetPlayer()->GetPosition() };
	Point2f position{ GetGameObject()->GetPosition() };

	return utils::GetDistance(playerPos, position) < enemy_05_player_range;
}

void Enemy05::Move(float elapsedSec)
{
	GameObject* parent{ GetGameObject() };
	Vector2f forward{ -parent->GetFrontVector() };

	parent->SetPosition(parent->GetPosition() + forward * enemy_05_speed * elapsedSec, true);
	SetAnimationState(Walking);
}

void Enemy05::Charge(float elapsedSec)
{
	if (m_ChargeTimer < 0) {
		m_ChargeTimer = 1.5f;

		Vector2f dir{ GetPlayer()->GetPosition() - GetGameObject()->GetPosition() };
		Vector2f forward{ -GetGameObject()->GetFrontVector() };

		float angle{ acosf(dir.DotProduct(forward) / (dir.Length() + forward.Length())) * (180.f / float(M_PI)) };


		int multiplyer{ angle > 90 ? -1 : 1 };
		GetGameObject()->m_Transform.Scale.x = multiplyer * GetGameObject()->m_Transform.Scale.x;

	}

	if (m_ChargeTimer > 0)
	{
		if (m_ChargeTimer < .5f)
		{
			SetAnimationState(PostCharge);
		}
		else if (m_ChargeTimer < 1.f)
		{
			SetAnimationState(Charging);
			GameObject* parent{ GetGameObject() };
			Vector2f forward{ -parent->GetFrontVector() };

			parent->SetPosition(parent->GetPosition() + forward * enemy_06_chargeSpeed * elapsedSec, true);
			m_Body->m_Velocity = Vector2f{};

		}
		else if (m_ChargeTimer < 1.5f)
		{
			SetAnimationState(PreCharge);
		}


		m_ChargeTimer -= elapsedSec;


	}
}
