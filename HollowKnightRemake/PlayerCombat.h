#pragma once
#include "Component.h"
#include "SoundEffect.h"

class PlayerAnimationHandler;
class ParticleSystem;
class PlayerMovement;
class Damageable;
class PhyicsBody;
class PlayerCombat final : public Component
{
public:
	PlayerCombat(int damage, float radius,float attackSpeed);
	
	void Start() override;
	void Update(float elpasedSec);
	void Draw() const;

	void KeyDownEvent(const SDL_KeyboardEvent& e) override;
	void KeyUpEvent(const SDL_KeyboardEvent& e) override;

	void MouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void MouseUpEvent(const SDL_MouseButtonEvent& e) override;

	void Hit(float stunTime = 1.0f);
private:
	void Attack();
	void CreateHitParticles();

	int m_Damage;
	float m_Radius;

	float m_AttackSpeed;
	float m_AttackTimer;

	bool m_Attacking;
	float m_AttackAnimationTime;
	
	Vector2f m_LookDirection;
	PhyicsBody* m_Body;

	PlayerAnimationHandler* m_AnimationHandler;
	PlayerMovement* m_PlayerMovement;
	Damageable* m_Damageable;

	GameObject* m_ParticleSystem;
	ParticleSystem* m_SlashParticleSystem;

	GameObject* m_HitParticleSystem;
	ParticleSystem* m_HitParticleSystemEffect;

	GameObject* m_SplashEffectParent;
	ParticleSystem* m_SplashEffect;

	SoundEffect* m_AttackSound;
	SoundEffect* m_HitSound;
};

