#include "pch.h"
#include "Enemy02.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "PhysicsEngine.h"
#include "PhyicsBody.h"
#include "PlayerCombat.h"
#include "GameObjectManager.h"


Enemy02::Enemy02() :
	Enemy(3),
	m_RotationCooldown{ -1 },
	m_RotateDirection{ 0 },
	m_StunTimer{ -1 },
	m_DieTimer{ 1 },
	m_Dead{ false },
	m_Body{ nullptr },
	m_Collider{ nullptr }
{
}

void Enemy02::Start()
{
	Enemy::Start();

	int angle{ rand() % 361 };
	m_Collider = GetComponent<Collider>();
	m_Body = GetComponent<PhyicsBody>();
	m_Body->SetEnabled(false);


	Texture* texture{ GetTextureManager()->GetTexture("Enemies/Enemy_02.png") };
	SpriteRenderer* renderer{ GetComponent<SpriteRenderer>() };
	renderer->SetTexture(texture);

	const float playRate{ 1.f / 10.f };

	Rectf source{ 0,0,texture->GetWidth() / 7, texture->GetHeight() / 5 };
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
				false
			}
		},
		{
			TurningDown,
			AnimationSet
			{
				TurningDown,
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
			TurningUp,
			AnimationSet
			{
				TurningUp,
				source,
				4,
				0,
				3,
				1.f / 2.f,
				false,
				false
			}
		},
		{
			Dead,
			AnimationSet
			{
				Dead,
				source,
				3,
				0,
				2,
				1.f / 5.f,
				false,
				false
			}
		},
	};

	LoadAnimation(renderer, animations);
}

void Enemy02::Update(float elpasedSec)
{
	Enemy::Update(elpasedSec);

	m_Collider->SetPosition(GetGameObject()->GetPosition());

	if (m_Dead)
	{
		m_DieTimer -= elpasedSec;
		SetAnimationState(Dead);
		if (m_DieTimer < 0)
		{
			GetGameObjectManager()->DeleteObject(GetGameObject());

		}
		return;
	}

	if (Rotating(elpasedSec))
	{
		return;
	}

	if (m_StunTimer > 0)
	{
		m_StunTimer -= elpasedSec;
		SetAnimationState(Hit);
		return;
	}

	std::vector<bool> results{ Detect() };

	if (!results[0] && (results[1] || results[2]))
	{
		SetAnimationState(Walking);
		Move(elpasedSec);
	}
	else
	{
		m_RotateDirection = results[0] && results[1] ? 1 : -1;

		m_RotationCooldown = 1;
	}

	CheckForPlayerDamage();
}

void Enemy02::Move(float elpasedSec)
{
	GameObject* parent{ GetGameObject() };
	PhysicsEngine* engine{ GetPhysicsEngine() };

	Vector2f forward{ parent->GetFrontVector() };
	Vector2f up{ parent->GetUpVector() };

	Point2f start{ parent->GetPosition() };
	Point2f end{ start + forward * enemy_02_detection_range };

	Collider* result{ nullptr };
	utils::HitInfo info;

	std::vector<Layers> layer{ Layers::Static };

	if (engine->Raycast(start, start + (-up) * enemy_02_detection_range, info, &result, layer, m_Collider))
	{
		Vector2f upwardPos{ up * m_Collider->GetHeight() * 0.5f };
		Vector2f forwardPos{ forward * enemy_02_speed * elpasedSec };
		parent->SetPosition(info.intersectPoint + upwardPos + forwardPos, true);
	}
	else {
		parent->SetPosition(start + forward * enemy_02_speed * elpasedSec, true);
	}
}

void Enemy02::Draw() const
{
	GameObject* parent{ GetGameObject() };

	Vector2f forward{ parent->GetFrontVector() };
	Vector2f up{ parent->GetUpVector() };

	Point2f start{ parent->GetPosition() };
	Point2f end{ start + forward * enemy_02_detection_range };

	utils::SetColor(Color4f{ 0,1,0,1 });

	utils::DrawLine(start, end);
	utils::DrawLine(start - (forward * (m_Collider->GetWidth() * 0.5f)), start - (forward * (m_Collider->GetWidth() * 0.5f)) + (-up) * enemy_02_detection_range);
	utils::DrawLine(start + (forward * m_Collider->GetWidth() * 0.2f), start + (forward * m_Collider->GetWidth() * 0.2f) + (-up) * enemy_02_detection_range);

	const Ellipsef circle{ parent->GetPosition(),enemy_02_damage_range,enemy_02_damage_range };
	utils::DrawEllipse(circle);
}

void Enemy02::CheckForPlayerDamage()
{
	GameObject* parent{ GetGameObject() };
	const Circlef circle{ parent->GetPosition(),enemy_02_damage_range };
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

void Enemy02::OnTakeDamage(int damage)
{
	Enemy::OnTakeDamage(damage);
	m_StunTimer = enemy_02_stun_durration;
}

void Enemy02::OnDead()
{
	Enemy::OnDead();

	m_DieTimer = 6;
	m_Dead = true;
	GetGameObject()->m_Transform.Rotation = 0;
	m_Body->SetEnabled(true);
	GetGameObject()->SetPosition(GetGameObject()->GetPosition() + GetGameObject()->GetUpVector() * 10, true);
	m_Body->AddForce(GetGameObject()->GetUpVector() * 200, true);
}

int Enemy02::GetTinValue()
{
	return enemy_02_tin_value;
}

bool Enemy02::Rotating(float elpasedSec)
{
	if (m_RotationCooldown > 0)
	{
		m_RotationCooldown -= elpasedSec;

		Enemy_02_AnimationStates state{ m_RotateDirection == 1 ? TurningUp : TurningDown };
		SetAnimationState(state);

		GameObject* parent{ GetGameObject() };
		PhysicsEngine* engine{ GetPhysicsEngine() };

		Vector2f up{ parent->GetUpVector() };

		Vector2f forward{ parent->GetFrontVector() };
		Point2f start{ parent->GetPosition() };

		if (state == TurningDown) {
			parent->SetPosition(start + (-up) * enemy_02_speed * elpasedSec, true);
		}

		if (m_RotationCooldown < 0)
		{
			if (m_RotateDirection == 1)parent->m_Transform.Rotation += 90;
			else parent->m_Transform.Rotation -= 90;

			return false;
		}
		return true;
	}
	return false;
}

std::vector<bool> Enemy02::Detect()
{
	GameObject* parent{ GetGameObject() };
	PhysicsEngine* engine{ GetPhysicsEngine() };

	Vector2f forward{ parent->GetFrontVector() };
	Vector2f up{ parent->GetUpVector() };

	Point2f start{ parent->GetPosition() };
	Point2f end{ start + forward * (m_Collider->GetWidth() * 0.5f + 1) };

	Collider* result{ nullptr };
	utils::HitInfo info;

	std::vector<Layers> layer{ Layers::Static };

	bool frontCheck{ engine->Raycast(start,end,info,&result,layer,m_Collider) };
	bool downCheck{ engine->Raycast(start - (forward * (m_Collider->GetWidth() * 0.5f)),start - (forward * (m_Collider->GetWidth() * 0.5f)) + (-up) * enemy_02_detection_range,info,&result,layer,m_Collider) };
	bool middleDownCheck{ engine->Raycast(start + (forward * m_Collider->GetWidth() * 0.2f),start + (forward * m_Collider->GetWidth() * 0.2f) + (-up) * enemy_02_detection_range,info,&result,layer,m_Collider) };

	return std::vector<bool>
	{
		frontCheck,
			downCheck,
			middleDownCheck
	};
}
