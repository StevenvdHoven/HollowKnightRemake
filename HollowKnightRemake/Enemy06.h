#pragma once
#include "Enemy.h"

#define Enemy_06_transitiontime 0.25f
#define Enemy_06_cooldowntime 3
#define Enemy_06_width 195
#define Enemy_06_height 540

enum Enemy_06_AnimationStates
{
	Hidden,
	Idle,
	GoingUp,
	GoingDown,
};

class ParticleSystem;
class Enemy06 : public Enemy
{
public:
	Enemy06();
	virtual ~Enemy06();


	void Start() override;
	void Update(float elpasedSec) override;
	void Draw() const;

	void CheckForPlayerDamage();

private:
	void LoadParticleEffects();

	bool m_IsUp;
	float m_CooldownTimer;
	float m_TransitionTimer;

	std::vector<Point2f> m_OverlapRect;

	ParticleSystem* m_RockPuddleEffect;
	ParticleSystem* m_ExplodeRockPuddleEffect;
	GameObject* m_RockPuddleVFXGameObject;
	GameObject* m_ExplodeRockPuddleVFXGameObject;
};

