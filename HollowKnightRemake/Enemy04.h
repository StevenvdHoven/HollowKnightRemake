#pragma once
#include "Enemy.h"

#define enemy_04_health 3
#define enemy_04_shoot_range 500
#define enemy_04_detection_range 800
#define enemy_04_lose_range 1000
#define enemy_04_min_direction_cooldown 2
#define enemy_04_max_direction_cooldown 6
#define enemy_04_max_speed 125.f
#define enemy_04_accelaration 10.f
#define enemy_04_damage_range 50
#define enemy_04_stun_time 1

#define enemy_04_shoot_time 1
#define enemy_04_shoot_cooldown 3

#define enemy_04_tin_value 6

enum Enemy_04_AnimationStates
{
	Flying,
	Spit,
	Spit2,
	Dying,
};

class PhyicsBody;
class Collider;
class ParticleSystem;
class Enemy04 final : public Enemy
{
public:
	Enemy04();
	virtual ~Enemy04();

	void Start() override;
	void Update(float elpasedSec) override;
	void Move(float elpasedSec);
	void Draw() const;

	void CheckForPlayerDamage();

	void OnTakeDamage(int damage) override;
	void OnDead() override;

protected:
	int GetTinValue() override;

private:
	bool SeesPlayer();
	void CheckForDirection();
	void CaculateRandomDiretion();

	void Shoot();

	Vector2f m_Direction;
	float m_StunTimer;
	bool m_Dead;
	float m_DieTimer;
	float m_DirectionTimer;
	float m_DetectionTimer;

	float m_ShootTime;
	float m_ShootCooldown;
	bool m_SeesPlayer;


	PhyicsBody* m_Body;
	Collider* m_Collider;

	GameObject* m_ShootParticleParent;
	ParticleSystem* m_ShootParticle;
};

