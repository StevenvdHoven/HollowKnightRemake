#pragma once
#include "Enemy.h"
#include "utils.h"

#define enemy_03_health 3
#define enemy_03_detection_range 80
#define enemy_03_min_direction_cooldown 2
#define enemy_03_max_direction_cooldown 6
#define enemy_03_max_speed 250.f
#define enemy_03_accelaration 20.f
#define enemy_03_damage_range 50
#define enemy_03_stun_time 1

#define enemy_03_tin_value 2

enum Enemy_03_AnimationStates 
{
	Flying,
	Hit,
	Dying,
};


class PhyicsBody;
class Collider;
class Enemy03 : public Enemy
{
public:
	Enemy03();

	void Start() override;
	void Update(float elpasedSec) override;
	void Move(float elpasedSec);
	void Draw() const;

	void CheckForPlayerDamage();

	void OnTakeDamage(int damage) override;
	void OnDead() override;


	//virtual void OnCollisionEnter(Collider* collider);
protected:
	int GetTinValue() override;

private:
	Collider* Detect();
	
	Vector2f m_Direction;
	float m_StunTimer;
	bool m_Dead;
	float m_DieTimer;
	float m_Scale;
	float m_RecalculateTimer;

	PhyicsBody* m_Body;
	Collider* m_Collider;
};

