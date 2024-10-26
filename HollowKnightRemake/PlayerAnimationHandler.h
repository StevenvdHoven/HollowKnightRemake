#pragma once
#include "Component.h"
#include <map>

enum PlayerAnimationState 
{
	Idle,
	Moving,
	AirBornUp,
	AirBornDown,
	Attacking,
};

class SpriteRenderer;
class PhyicsBody;
class SoundEffect;
class PlayerAnimationHandler : public Component
{
public:
	PlayerAnimationHandler();

	void Start() override;
	void Update(float elpasedSec) override;

	void UpdateAnimation();
	void SetMoving(bool moving);
	void SetGrounded(bool grounded);
	void SetAttacking(bool attacking);

	Rectf GetSourceFrame(int frame);


private:
	void CheckAnimation();


	float m_FrameWidth;
	float m_FrameHeight;

	float m_FrameRate;
	float m_Timer;

	int m_Frame;
	int m_StartFrame;
	int m_EndFrame;

	bool m_IsMoving;
	bool m_IsGrounded;
	bool m_IsAttacking;
	bool m_Loop;

	PlayerAnimationState m_State;
	std::map<PlayerAnimationState, SoundEffect*> m_SoundEffects;

	SpriteRenderer* m_Renderer;
	PhyicsBody* m_Body;
};

