#include "pch.h"
#include "Enemy01.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "PhysicsEngine.h"
#include <iostream>
#include "PhyicsBody.h"
#include <map>
#include "PlayerCombat.h"
#include "GameObjectManager.h"

Enemy01::Enemy01() :
	Enemy(enemy_01_health),
	m_Direction{ -1 },
	m_DetectionCooldown{ .5f },
	m_StunTimer{ 0 },
	m_DieTimer{ 1 },
	m_Dead{ false },
	m_Body{ nullptr },
	m_Collider{ nullptr }
{
}

void Enemy01::Start()
{
	Enemy::Start();

	m_Body = GetComponent<PhyicsBody>();
	m_Collider = GetComponent<Collider>();

	Texture* texture{ GetTextureManager()->GetTexture("Enemies/Enemy_01.png") };
	SpriteRenderer* renderer{ GetComponent<SpriteRenderer>() };
	renderer->SetTexture(texture);

	const float playRate{ 1.f / 5.f };

	Rectf source{ 0,0,texture->GetWidth() / 4, texture->GetHeight() / 4 };
	std::map<int, AnimationSet> animations
	{
		{
			Walking,
			AnimationSet
			{
				Walking,
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
			Turning,
			AnimationSet
			{
				Turning,
				source,
				1,
				0,
				3,
				1.f / 3.f,
				false,
				false
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
				3,
				playRate,
				false
			}
		},
		{
			Died,
			AnimationSet
			{
				Died,
				source,
				3,
				0,
				2,
				playRate,
				false,
				false
			}
		},
	};

	LoadAnimation(renderer, animations);
}

void Enemy01::Update(float elpasedSec)
{
	Enemy::Update(elpasedSec);

	if (m_Dead)
	{
		m_Body->m_Velocity = Vector2f{};
		if (m_DieTimer < 4.f) {
			SetAnimationState(Died);
		}
		else {
			SetAnimationState(Dying);
		}

		if (m_DieTimer < 0)
		{
			GetGameObjectManager()->DeleteObject(GetGameObject());
		}
		else
		{
			m_DieTimer -= elpasedSec;
		}
		return;
	}


	if (m_DetectionCooldown > 0)
	{
		m_DetectionCooldown -= elpasedSec;
		if (m_DetectionCooldown < 0)
		{
			SetAnimationState(Walking);
		}
		else {
			SetAnimationState(Turning);
		}
	}

	if (m_StunTimer <= 0) m_Body->m_Velocity.x = m_Direction * m_Body->m_Mass * enemy_01_speed;
	else m_StunTimer -= elpasedSec;

	if (Detect() && m_DetectionCooldown < 0)
	{
		m_DetectionCooldown = 0.5f;
		if (m_Direction == 1) m_Direction = -1;
		else m_Direction = 1;
		GetGameObject()->m_Transform.Scale.x = -m_Direction;
		m_StunTimer = enemy_01_stun_durration;
		m_Body->m_Velocity.x = 0;
	}

	CheckForPlayerDamage();
}

void Enemy01::Draw() const
{
	GameObject* parent{ GetGameObject() };
	Vector2f dir{ -parent->GetFrontVector() };
	Vector2f down{ -parent->GetUpVector() };

	Point2f start{ parent->GetPosition() };
	Point2f end{ parent->GetPosition() + dir * enemy_01_range };

	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::DrawLine(start, end);
	utils::DrawLine(end, end + down * enemy_01_range);

	const Ellipsef circle{ parent->GetPosition(),enemy_01_damage_range,enemy_01_damage_range };
	utils::DrawEllipse(circle);
}

void Enemy01::CheckForPlayerDamage()
{
	GameObject* parent{ GetGameObject() };
	const Circlef circle{ parent->GetPosition(),enemy_01_damage_range };
	if (isOverlappingWithPlayer(circle))
	{

		PlayerCombat* playerCombat{ GetPlayer()->GetComponent<PlayerCombat>() };
		
		std::cout << "Player hit" << std::endl;
		playerCombat->Hit(1.5f);

		PhyicsBody* playerBody{ GetPlayer()->GetComponent<PhyicsBody>() };
		Vector2f dir{ playerCombat->GetGameObject()->GetPosition() - parent->GetPosition() };
		dir = dir / dir.Length();
		playerBody->AddForce(dir * 200, true);

	}
}

void Enemy01::OnTakeDamage(int damage)
{
	Enemy::OnTakeDamage(damage);
	GameObject* parent{ GetGameObject() };
	Vector2f dir{ GetPlayer()->GetPosition() - parent->GetPosition() };
	dir.y = 2;
	dir = dir / dir.Length();
	m_Body->AddForce(-dir * 200, true);
	m_StunTimer = enemy_01_stun_durration;
}

void Enemy01::OnDead()
{
	Enemy::OnDead();

	m_Dead = true;
	m_DieTimer = 4.5f;
}

void Enemy01::OnCollisionEnter(Collider* collider)
{

}

int Enemy01::GetTinValue()
{
	return enemy_01_tin_value;
}


bool Enemy01::Detect()
{
	GameObject* parent{ GetGameObject() };
	Vector2f dir{ -parent->GetFrontVector() };
	Vector2f down{ -parent->GetUpVector() };

	PhysicsEngine* engine{ GetPhysicsEngine() };

	utils::HitInfo info;
	Collider* resultCollider{ nullptr };

	std::vector<Layers> layer{ Layers::Static };

	Point2f start{ parent->GetPosition() };
	Point2f end{ parent->GetPosition() + dir * enemy_01_range };

	bool forwardResult = engine->Raycast(
		start,
		end,
		info,
		&resultCollider,
		layer,
		m_Collider);
	if (forwardResult) {
		forwardResult = true;
	}

	bool downResult = !engine->Raycast(
		end,
		end + down * enemy_01_range,
		info,
		&resultCollider,
		layer,
		m_Collider);

	return forwardResult || downResult;
}
