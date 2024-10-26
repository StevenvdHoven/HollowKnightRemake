#include "pch.h"
#include "KillZone.h"
#include "Collider.h"
#include "PlayerCombat.h"
#include "LevelManager.h"
#include "PhyicsBody.h"

KillZone::KillZone():
	m_Hit{false}
{
}

KillZone::~KillZone()
{
}

void KillZone::Start()
{
	GetComponent<Collider>()->m_OnCollisions.push_back([this](Collider* collider) {OnTriggerEnter(collider); });
}

void KillZone::OnTriggerEnter(Collider* collider)
{
	if (m_Hit)
	{
		return;
	}

	if (collider->GetGameObject()->m_Tag == "player")
	{
		PlayerCombat* combat{ collider->GetComponent<PlayerCombat>() };
		if (combat != nullptr)
		{
			combat->Hit(1);
			Vector2f dir{ collider->GetPosition() - GetGameObject()->GetPosition() };
			dir = dir / dir.Length();
			collider->GetComponent<PhyicsBody>()->AddForce(dir * 200, true);

			GetLevelManager()->LoadLevel(GetLevelManager()->GetCurrentLevel());

			m_Hit = true;
		}
	}
}
