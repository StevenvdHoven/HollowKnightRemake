#pragma once
#include "Enemy.h"

#define enemy_01_health 3
#define enemy_01_speed 5
#define enemy_01_range 90
#define enemy_01_damage_range 50
#define enemy_01_stun_durration 1

#define enemy_01_tin_value 2


enum Enemy_01_AnimationStates 
{
	Walking,
	Turning,
	Dying,
	Died,
};

class PhyicsBody;
class Enemy01 : Enemy
{
public:
	Enemy01();

	void Start() override;
	void Update(float elpasedSec) override;
	void Draw() const override;

	void CheckForPlayerDamage();

	void OnTakeDamage(int damage) override;
	void OnDead() override;

	void OnCollisionEnter(Collider* collider) override;
protected:
	int GetTinValue() override;

private:
	bool Detect();
	int m_Direction;
	float m_DetectionCooldown;
	float m_StunTimer;
	float m_DieTimer;
	bool m_Dead;

	PhyicsBody* m_Body;
	Collider* m_Collider;

	

};

