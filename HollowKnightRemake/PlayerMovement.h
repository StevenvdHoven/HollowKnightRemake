#pragma once
#include "Component.h"
#include "Collider.h"
#include "SoundEffect.h"

#define player_jump_mass 50
#define player_normal_mass 140

class PhyicsBody;
class PlayerMovement : public Component
{
public:
	PlayerMovement(float speed,float jumpforce);


	void Start() override;
	void Update(float elpasedSec) override;
	void KeyDownEvent(const SDL_KeyboardEvent& e) override;
	void KeyUpEvent(const SDL_KeyboardEvent& e) override;

	void OnCollisionEnter(Collider* collider) override;
	void OnCollisionStay(Collider* collider) override;
	void OnCollisionExit(Collider* collider) override;

	void Stun(float time);
	bool IsStunned();

	void SetPause(bool pause);

private:
	float m_Speed;
	float m_JumpForce;
	float m_InputDirectionX;

	float m_StunTime;
	bool m_Stunned;
	bool m_Paused;

	PhyicsBody* m_Body;

	bool m_CanJump;
	SoundEffect* m_JumpSound;

};

