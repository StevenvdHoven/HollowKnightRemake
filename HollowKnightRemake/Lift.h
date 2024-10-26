#pragma once
#include "Component.h"
#include "Curve.h"

#define lift_range 20
#define lift_recovertime 1.f

#define lift_big_preset Rectf{0,0,420,70}
#define lift_small_preset Rectf{0,0, 280,70}

class RectCollider;
class Lift final : public Component
{
public:
	Lift(bool small);
	virtual ~Lift();

	void Start() override;
	void Update(float elapsedSec) override;
	void Draw() const override;

	void CheckForStepOn();
	void StepOnLift();

private:
	bool m_IsSmall;
	bool m_CanStepOn;
	Point2f m_StartPosition;
	float m_Timer;

	Curve m_PlatformCurve;
	RectCollider* m_Collider;
	
};

