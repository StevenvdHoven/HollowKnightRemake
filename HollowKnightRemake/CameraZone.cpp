#include "pch.h"
#include "CameraZone.h"
#include "Collider.h"
#include "CameraManager.h"
#include "GameObjectManager.h"
#include "Curve.h"


CameraZone::CameraZone(bool lockX, bool lockY, bool centered):
	m_SelfCollider{nullptr},
	m_PlayerCollider{nullptr},
	m_LockX{lockX},
	m_LockY{lockY},
	m_Centered{centered}
{
}

CameraZone::~CameraZone()
{
	GetCameraManager()->RemoveZone(this);
}

void CameraZone::Start()
{
	m_SelfCollider = GetComponent<Collider>();
	m_SelfCollider->SetEnabled(false);
}

void CameraZone::Update(float elapsedSec)
{
	if (m_PlayerCollider == nullptr)
	{
		GameObject* player{ GetGameObjectManager()->GetObjectBasedOfTag("player") };
		m_PlayerCollider = player->GetComponent<Collider>();
	}

	if (m_PlayerCollider->CheckCollision(m_SelfCollider))
	{
		GetCameraManager()->SetZone(this);
		m_LastPlayerPosition = m_PlayerCollider->GetPosition();
		
		if (!m_ZonedIn)
		{
			m_ZonedIn = true;
			m_LerpProcess = 1.f;
		}
	}
	else
	{
		GetCameraManager()->RemoveZone(this);
		m_ZonedIn = false;
	}

	if (m_ZonedIn) 
	{
		if (m_LerpProcess > 0) {
			m_LerpProcess -= elapsedSec;
		}
	}
}

void CameraZone::Draw() const
{
}

Point2f CameraZone::GetPosition()
{
	if (m_LerpProcess <= 0)
	{
		return m_SelfCollider->GetPosition();
	}

	return m_LastPlayerPosition + (m_SelfCollider->GetPosition() - m_LastPlayerPosition) * curve_slope_up.GetValue(1.f - m_LerpProcess);
}

bool CameraZone::LockedX()
{
	return m_LockX;
}

bool CameraZone::LockedY()
{
	return m_LockY;
}

bool CameraZone::IsCentered()
{
	return m_Centered;
}
