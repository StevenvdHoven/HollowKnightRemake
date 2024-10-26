#pragma once
#include "Enemy.h"

#define enemy_05_health 3
#define enemy_05_speed 40
#define enemy_06_chargeSpeed 400
#define enemy_05_range 90
#define enemy_05_player_range 200
#define enemy_05_attack_range
#define enemy_05_damage_range 50
#define enemy_05_stun_durration 1

#define enemy_05_tin_value 3

enum Enemy_05_AnimationStates
{
	Idle,
	Walking,
	Turning,
	PreCharge,
	Charging,
	PostCharge,
	Dying,
	Died,
};

class PhyicsBody;
class Enemy05 : public Enemy
{
public:
	Enemy05();

	void Start() override;
	void Update(float elpasedSec) override;
	void Draw() const override;

	void CheckForPlayerDamage();

	void OnTakeDamage(int damage) override;
	void OnDead() override;

protected:
	int GetTinValue() override;

private:
	bool CheckForPlayer();
	void Charge(float elapsedSec);

	bool Detect();
	void Move(float elapsedSec);

	float m_TurnTimer;
	float m_StunTimer;
	float m_ChargeTimer;
	float m_DieTimer;
	bool m_Dead;

	PhyicsBody* m_Body;
	Collider* m_Collider;
};

