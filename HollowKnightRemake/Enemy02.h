#pragma once
#include "Enemy.h"
#include "utils.h"

#define enemy_02_speed 50
#define enemy_02_detection_range 80
#define enemy_02_damage_range 50
#define enemy_02_stun_durration 1

#define enemy_02_tin_value 2;

enum Enemy_02_AnimationStates
{
	Walking,
	TurningDown,
	TurningUp,
	Hit,
	Dead,
};

class PhyicsBody;
class Enemy02 : public Enemy
{
public:
	Enemy02();

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
	bool Rotating(float elpasedSec);
	std::vector<bool> Detect();


	float m_RotationCooldown;
	int m_RotateDirection;
	float m_StunTimer;
	float m_DieTimer;
	bool m_Dead;

	PhyicsBody* m_Body;
	Collider* m_Collider;
};

