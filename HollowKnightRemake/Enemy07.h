#pragma once
#include "Enemy.h"

#define enemy_07_health 3
#define enemy_07_detection_range 500
#define enemy_07_lose_range 800
#define enemy_07_min_direction_cooldown 2
#define enemy_07_max_direction_cooldown 6
#define enemy_07_max_speed 250.f
#define enemy_07_accelaration 20.f
#define enemy_07_damage_range 50
#define enemy_07_stun_time 1

#define enemy_07_tin_value 3

enum Enemy_07_AnimationStates
{
	Flying,
	Spotted,
	Bite,
	Dying,
};


class PhyicsBody;
class Collider;

class Enemy07 final : public Enemy
{
public:
	Enemy07();

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

	Vector2f m_Direction;
	float m_StunTimer;
	bool m_Dead;
	float m_DieTimer;
	float m_Scale;
	float m_RecalculateTimer;

	float m_SpotTimer;
	bool m_AlreadySpotted;

	bool m_SeesPlayer;

	PhyicsBody* m_Body;
	Collider* m_Collider;
};

