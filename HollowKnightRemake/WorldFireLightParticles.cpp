#include "pch.h"
#include "WorldFireLightParticles.h"
#include "ParticleSystem.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"

WorldFireLightParticles::WorldFireLightParticles()
{

}

void WorldFireLightParticles::Start()
{
	ParticleSystem* particleSystem{ new ParticleSystem
		{
			GetTextureManager()->GetTexture("VFX/White_particle.png"),
			1,
			1,
			1,
			1000,
			10,
			true,
			true,
		}
	};

	ParticleSpawnRules spawnrules
	{
		.5f,
		20,
		50,
		true
	};

	std::function<void(Particle&)> onSpawnRule
	{
		[this](Particle& particle)
		{

			Point2f pos{float(cosf(rand() % 361) * 3000),float(cosf(rand() % 361) * (2000 * 0.5f))};
			particle.Position = pos;

			int randangle{ rand() % 361 };
			particle.Velocity = Vector2f{ cosf(randangle), sinf(randangle) };
			particle.Gravity = Vector2f{ 0,-9.81 };
			particle.Mass = .1f;
			particle.LifeTime = 5;

			particle.OpacityOverTime = curve_half_circle;
			particle.Scale = 0.1f;
		}
	};

	particleSystem->SetSpawnRule(spawnrules);
	particleSystem->SetSpawnRule(onSpawnRule);
	AddComponent(particleSystem);
	

	GameObject::Start();
	particleSystem->SetOrder(-22);
}
