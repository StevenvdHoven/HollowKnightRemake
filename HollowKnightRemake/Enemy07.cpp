#include "pch.h"
#include "Enemy07.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"
#include "PlayerCombat.h"
#include "RectCollider.h"
#include "GameObjectManager.h"

Enemy07::Enemy07():
	Enemy(enemy_07_health),
	m_Direction{},
	m_StunTimer{ -1 },
	m_Dead{ false },
	m_DieTimer{ 0 },
	m_Scale{ 1 },
	m_RecalculateTimer{ -1 },
	m_SeesPlayer{false},
	m_Body{ nullptr },
	m_Collider{ nullptr }
{
}

void Enemy07::Start()
{
	Enemy::Start();

	m_Body = GetComponent<PhyicsBody>();
	m_Collider = GetComponent<Collider>();

	m_Scale = GetGameObject()->m_Transform.Scale.x;


	Texture* texture{ GetTextureManager()->GetTexture("Enemies/Enemy_07.png") };
	SpriteRenderer* renderer{ GetComponent<SpriteRenderer>() };
	renderer->SetTexture(texture);

	const float playRate{ 1.f / 10.f };

	Rectf source{ 0,0,texture->GetWidth() / 5, texture->GetHeight() / 5 };
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
				5,
				playRate,
				true,
				false
			}
		},
		{
			Spotted,
			AnimationSet
			{
				Spotted,
				Rectf{ 0,0,texture->GetWidth() / 4, texture->GetHeight() / 5 },
				2,
				0,
				4,
				1.f / 10.f,
				false,
				false,
			}
		},
		{
			Bite,
			AnimationSet
			{
				Bite,
				Rectf{ 0,0,texture->GetWidth() / 4, texture->GetHeight() / 5 },
				3,
				0,
				4,
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
				Rectf{ 0,0,texture->GetWidth() / 4, texture->GetHeight() / 5 },
				4,
				0,
				3,
				1.f / 5.f,
				false,
				false
			}
		},
	};

	LoadAnimation(renderer, animations);
}

void Enemy07::Update(float elpasedSec)
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

	if (m_SpotTimer > 0)
	{
		m_SpotTimer -= elpasedSec;
	}

	if (SeesPlayer())
	{
		if (!m_AlreadySpotted)
		{
			m_SpotTimer = .75f;
			m_AlreadySpotted = true;
		}
	}
	else
	{
		m_AlreadySpotted = false;
	}

	if (m_StunTimer <= 0)
	{
		Move(elpasedSec);
	}
	else
	{
		m_StunTimer -= elpasedSec;
	}
	

	CheckForPlayerDamage();
}

void Enemy07::Move(float elpasedSec)
{
	m_Direction = GetPlayer()->GetPosition() - GetGameObject()->GetPosition();
	m_Direction = m_Direction / m_Direction.Length();

	int scaler{ m_Direction.x > 0 ? -1 : 1 };
	GetGameObject()->m_Transform.Scale.x = abs(GetGameObject()->m_Transform.Scale.x) * scaler;
	if (m_SpotTimer > 0)
	{
		SetAnimationState(Spotted);
		return;
	}


	SetAnimationState(Flying);
	if (m_SeesPlayer)
	{
		m_Direction = GetPlayer()->GetPosition() - GetGameObject()->GetPosition();
		m_Direction = m_Direction / m_Direction.Length();

		m_Body->AddForce(m_Direction * enemy_07_max_speed, true);
	}
}

void Enemy07::Draw() const
{
	Enemy::Draw();

	const Ellipsef circle{ GetGameObject()->GetPosition(),enemy_07_damage_range,enemy_07_damage_range };
	utils::DrawEllipse(circle);

	Ellipsef ellipse{ GetGameObject()->GetPosition(),enemy_07_detection_range,enemy_07_detection_range };
	utils::SetColor(Color4f{ 0, 1, 0, 1 });
	utils::DrawEllipse(ellipse);

	ellipse = Ellipsef{ GetGameObject()->GetPosition(),enemy_07_lose_range,enemy_07_lose_range };
	utils::SetColor(Color4f{ 0,0,1,1 });
	utils::DrawEllipse(ellipse);
}

void Enemy07::CheckForPlayerDamage()
{
	GameObject* parent{ GetGameObject() };
	const Circlef circle{ parent->GetPosition(),enemy_07_damage_range };
	if (isOverlappingWithPlayer(circle))
	{

		PlayerCombat* playerCombat{ GetPlayer()->GetComponent<PlayerCombat>() };

		playerCombat->Hit(1.5f);

		PhyicsBody* playerBody{ GetPlayer()->GetComponent<PhyicsBody>() };
		Vector2f dir{ playerCombat->GetGameObject()->GetPosition() - parent->GetPosition() };
		dir = dir / dir.Length();
		playerBody->AddForce(dir * 200, true);
		m_Body->AddForce(-dir * 100, true);
		m_StunTimer = enemy_07_stun_time;
	}
}

void Enemy07::OnTakeDamage(int damage)
{
	Enemy::OnTakeDamage(damage);
	GameObject* parent{ GetGameObject() };
	Vector2f dir{ GetPlayer()->GetPosition() - parent->GetPosition() };
	dir = dir / dir.Length();
	m_Body->AddForce(-dir * 200, true);

	m_StunTimer = enemy_07_stun_time;
}

void Enemy07::OnDead()
{
	Enemy::OnDead();

	m_Dead = true;
	m_DieTimer = .75f;
}

int Enemy07::GetTinValue()
{
	return enemy_07_tin_value;
}

bool Enemy07::SeesPlayer()
{
	const float distance{ utils::GetDistance(GetPlayer()->GetPosition(),GetGameObject()->GetPosition()) };

	if (m_SeesPlayer)
	{
		m_SeesPlayer = !(distance > enemy_07_lose_range);
	}
	else
	{
		m_SeesPlayer = distance < enemy_07_detection_range;
	}

	return m_SeesPlayer;
}
