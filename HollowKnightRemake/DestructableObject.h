#pragma once
#include "Component.h"
#include <vector>

#define destructable_fadeawaytime 10

class ParticleSystem;
class SpriteRenderer;
class Damageable;
class SoundEffect;
class DestructableObject : public Component
{
public:
	DestructableObject();
	virtual ~DestructableObject();

	void Start() override;
	void Update(float elapsedSec) override;

	void Link(GameObject* taggedGameObject);
	void Link(ParticleSystem* particleSystem);
	void AddAudio(SoundEffect* sfx);
	void AddAudio(const std::string& filepathSFX);
private:
	bool m_Destroyed;
	float m_Timer;

	GameObject* m_LinkedMapGameObject;
	ParticleSystem* m_System;
	Damageable* m_Damageable;
	Collider* m_Collider;

	std::vector<SoundEffect*> m_SoundEffects;
};

