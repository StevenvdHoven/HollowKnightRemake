#include "pch.h"
#include "EnemySplashParticleEffect.h"
#include "TextureManager.h"
#include "ParticleSystem.h"
#include "GameObjectManager.h"
#include <functional>
#include "Curve.h"


EnemySplashParticleEffect::EnemySplashParticleEffect()
{
}

void EnemySplashParticleEffect::Start()
{
	ParticleSystem* splashEffect{ new ParticleSystem
		{
				GetTextureManager()->GetTexture("VFX/orange_smoke.png"),
				2,
				3,
				6,
				20,
				0.2f,
				false,
		}
	};

	m_Systems.push_back(splashEffect);

	ParticleSpawnRules spawnRules
	{
		20,
		10,
		20,
		true
	};

	std::function<void(Particle& particle)> onSpawnRules
	{
		[this](Particle& particle)
		{
			int randAngle{rand() % 361};
			Vector2f velocity{cosf(randAngle) * 200, sinf(randAngle) * 200};

			particle.Velocity = velocity;
			particle.Rotation = float(rand() % 361);
			particle.RotationVelocity = int(rand() % 3 - 1) * 80;

			std::vector<int> frames{ 0,2 };
			int randomIndex{ int(rand() % frames.size()) };
			particle.Frame = frames[randomIndex];
			particle.Animate = false;
			particle.LifeTime = 2;

			particle.SpeedOverTime = curve_slope_down;
			particle.OpacityOverTime = curve_slope_down;
			
			particle.TargetGravity = Vector2f{ 0,9.81 };
			particle.GravityOverTime = curve_default;
			particle.Mass = 10;

			particle.Scale = float(int(rand() % 21) * 0.12f);
		}
	};

	GameObject* splashEffectParent{new GameObject()};
	splashEffectParent->AddComponent(splashEffect);

	splashEffect->SetSpawnRule(spawnRules);
	splashEffect->SetSpawnRule(onSpawnRules);

	GetGameObjectManager()->AddObject(splashEffectParent);

	ParticleSystem* hitCrackSystem{ new ParticleSystem
		{
			GetTextureManager()->GetTexture("VFX/hit_crack.png"),
			1,
			3,
			3,
			0.3f,
			0.1f,
			false,
			false,
		}
	};
	m_Systems.push_back(hitCrackSystem);

	spawnRules = ParticleSpawnRules
	{
		20,
		1,
		1,
		true,
	};

	std::function<void(Particle&)> onSpawnRule
	{
		[](Particle& particle)
		{
			particle.Scale = 0.8f;
		}
	};

	GameObject* hitEffectParent{ new GameObject() };

	hitEffectParent->AddComponent(hitCrackSystem);
	hitCrackSystem->SetSpawnRule(spawnRules);
	GetGameObjectManager()->AddObject(hitEffectParent);
}

void EnemySplashParticleEffect::Play(const Point2f& pos) const
{
	for (int index{ 0 }; index < m_Systems.size(); ++index)
	{
		m_Systems[index]->GetGameObject()->SetPosition(pos);
		m_Systems[index]->Play();
	}
}
