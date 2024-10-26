#pragma once
#include "Component.h"
#define max_player_distance 1080 * 0.3f

class CameraManager;
class CameraFollower : public Component
{
public:
	CameraFollower(float speed, GameObject* target);

	void Start() override;
	void Update(float elapsedSec) override;
	void Zoom(float zoom);

private:
	float m_CameraPercentage;
	float m_CameraSpeed;
	GameObject* m_Target;
	CameraManager* m_Manager;
	Point2f m_LastPosition;
};

