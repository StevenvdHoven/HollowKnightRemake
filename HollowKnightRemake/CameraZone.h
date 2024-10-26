#pragma once
#include "Component.h"
#include "utils.h"

class Collider;
class CameraZone final : public Component
{
public:
	CameraZone(bool lockX = true, bool lockY = true, bool centerd = false);
	virtual ~CameraZone();

	void Start() override;
	void Update(float elapsedSec) override;
	void Draw() const override;

	Point2f GetPosition();

	bool LockedX();
	bool LockedY();
	bool IsCentered();

private:
	Collider* m_SelfCollider;
	Collider* m_PlayerCollider;

	float m_LerpProcess;
	bool m_ZonedIn;

	Point2f m_LastPlayerPosition;

	bool m_LockX;
	bool m_LockY;
	bool m_Centered;
};

