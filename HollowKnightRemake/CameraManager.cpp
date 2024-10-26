#include "pch.h"
#include "CameraManager.h"
#include "utils.h"
#include "CameraZone.h"
#include "Vector2f.h"

CameraManager::CameraManager(const Rectf& viewport) :
	m_CameraPosition{},
	m_CameraRotation{ 0 },
	m_Scale{ 1 },
	m_ViewPort{ viewport },
	m_CurrentZone{ nullptr }
{
}

void CameraManager::SetCameraPosition(const Point2f& pos, bool force)
{
	if (m_CurrentZone != nullptr && !force)
	{
		if (m_CurrentZone->IsCentered())
		{
			m_CameraPosition = -Vector2f(m_CurrentZone->GetPosition());
			return;
		}

		if (!m_CurrentZone->LockedX())
		{
			m_CameraPosition.x = pos.x;
		}
		if (!m_CurrentZone->LockedY())
		{
			m_CameraPosition.y = pos.y;
		}
	}
	else
	{
		m_CameraPosition = Vector2f(pos);
	}
}

void CameraManager::SetCameraRotation(float rot)
{
	m_CameraRotation = rot;
}

void CameraManager::SetZoom(float zoom)
{
	m_Scale = zoom;
}

Point2f CameraManager::GetCameraPosition()
{
	return Point2f(m_CameraPosition);
}

float CameraManager::GetCameraRotation()
{
	return m_CameraRotation;
}

float CameraManager::GetZoom()
{
	return m_Scale;
}

Transform CameraManager::GetTransform()
{
	Rectf viewport{ 0,0,1920,1080 };

	Transform trans;
	trans.Position = m_CameraPosition;
	trans.Rotation = m_CameraRotation;
	trans.Scale = Vector2f(1.f, 1.f) * m_Scale;
	return trans;
}

void CameraManager::SetZone(CameraZone* newZone)
{
	m_CurrentZone = newZone;
}

void CameraManager::RemoveZone(CameraZone* zone)
{
	if (m_CurrentZone == zone)
	{
		m_CurrentZone = nullptr;
	}
}
