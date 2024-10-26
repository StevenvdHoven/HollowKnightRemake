#include "pch.h"
#include "Lift.h"
#include "RectCollider.h"
#include "PhysicsEngine.h"

Lift::Lift(bool small) :
	m_IsSmall{ small },
	m_CanStepOn{ true },
	m_Timer{-1},
	m_PlatformCurve{curve_half_circle},
	m_Collider{nullptr}
{
}

Lift::~Lift()
{
	m_Collider = nullptr;
}

void Lift::Start()
{
	m_Collider = (RectCollider*)GetComponent<Collider>();
	m_Collider->SetPosition(GetGameObject()->GetPosition() + Vector2f(0, 15));
	if (m_IsSmall)
	{
		m_Collider->SetWidth(lift_small_preset.width);
		m_Collider->SetHeight(lift_small_preset.height);
	}
	else
	{
		m_Collider->SetWidth(lift_big_preset.width);
		m_Collider->SetHeight(lift_big_preset.height);
	}


	m_StartPosition = GetGameObject()->GetPosition();
}

void Lift::Update(float elapsedSec)
{
	CheckForStepOn();

	if (m_Timer >= 0)
	{
		m_Timer -= elapsedSec;
		const float percentile{ 1 - (m_Timer / lift_recovertime) };
		const float curveValue{ m_PlatformCurve.GetValue(percentile) };
		Vector2f lerpedPosition = Vector2f::Lerp(Vector2f(m_StartPosition), Vector2f(m_StartPosition) + Vector2f{ 0,-1 } * lift_range,curveValue);
		
		GetGameObject()->SetPosition(Point2f(lerpedPosition), true);
	}
	else {
		GetGameObject()->SetPosition(m_StartPosition, true);
	}

	//m_Collider->SetPosition(GetGameObject()->GetPosition());
}

void Lift::Draw() const
{
	const float percentile{ 1 - (m_Timer / lift_recovertime) };
	const float curveValue{ m_PlatformCurve.GetValue(percentile) };
	Vector2f lerpedPosition = Vector2f::Lerp(Vector2f(m_StartPosition), Vector2f(m_StartPosition) + Vector2f{ 0,-1 } *lift_range, curveValue);
	 
	utils::SetColor(Color4f{ 0,0,1,1 });
	utils::DrawLine(m_StartPosition, Point2f(lerpedPosition));

}

void Lift::CheckForStepOn()
{
	

	Rectf bounds{ m_Collider->GetRect() };
	bounds.bottom += 20;

	Collider* result{ nullptr };
	const std::vector<Layers> layers{ World };

	PhysicsEngine* engine{ GetPhysicsEngine() };
	if (engine->BoxCast(bounds, &result, layers, m_Collider))
	{
		if (result->GetGameObject()->m_Tag == "player" && m_CanStepOn)
		{
			StepOnLift();
		}
		
	}

	if (result == nullptr && !m_CanStepOn)
	{
		m_CanStepOn = true;
	}
	
	
}

void Lift::StepOnLift()
{
	if (m_Timer > 0 || !m_CanStepOn)
	{
		return;
	}
	
	m_CanStepOn = false;
	m_Timer = lift_recovertime;
}


