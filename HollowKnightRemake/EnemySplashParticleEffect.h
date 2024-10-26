#pragma once
#include "GameObject.h"
#include <vector>

class ParticleSystem;
class EnemySplashParticleEffect : public GameObject
{
public:
	EnemySplashParticleEffect();

	void Start() override;
	void Play(const Point2f& pos) const;

private:
	std::vector<ParticleSystem*> m_Systems;
};

