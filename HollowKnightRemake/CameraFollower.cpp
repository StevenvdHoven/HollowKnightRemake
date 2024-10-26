#include "pch.h"
#include "CameraFollower.h"
#include "CameraManager.h"
#include "utils.h"
#include <iostream>

CameraFollower::CameraFollower(float speed, GameObject* target) :
	m_CameraPercentage{ 0 },
	m_CameraSpeed{ speed },
	m_Target{ target },
	m_Manager{ nullptr },
	m_LastPosition{target->GetPosition()}
{
}

void CameraFollower::Start()
{
	m_Manager = GetCameraManager();
	m_Manager->SetCameraPosition(Point2f(-Vector2f(m_Target->GetPosition())),true);
}

void CameraFollower::Update(float elapsedSec)
{
	Vector2f start{ m_Manager->GetCameraPosition() };
	Vector2f end{ m_Target->GetPosition() };
	
	float distance{ utils::GetDistance(Point2f(start), Point2f(-end))};
	if (distance > 10)
	{
		Vector2f direction{ (-end) - start};
		direction = direction / direction.Length();

		if (distance > max_player_distance)
		{
			m_Manager->SetCameraPosition(Point2f(-Vector2f(m_Target->GetPosition() + direction * (max_player_distance))));
		}
		else
		{
			m_Manager->SetCameraPosition(Point2f(start) + direction * m_CameraSpeed * elapsedSec);
		}

		
		
	}
	m_LastPosition = Point2f(-end);
	
	//m_CameraPercentage += elapsedSec * m_CameraSpeed;
	//Point2f position{ Vector2f::Lerp(start, -end,m_CameraPercentage) };
	//m_Manager->SetCameraPosition(position);
	
}

void CameraFollower::Zoom(float zoom)
{
	m_Manager->SetZoom(zoom);
}
