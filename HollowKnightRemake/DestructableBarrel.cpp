#include "pch.h"
#include "DestructableBarrel.h"
#include "DestructableObject.h"
#include "RectCollider.h"
#include "ParticleSystem.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "Damageable.h"
#include "PhysicsEngine.h"
#include "GameObjectManager.h"
#include "AudioManager.h"
#include <functional>

DestructableBarrel::DestructableBarrel()
{
	m_Name = "Destructable_Barrel";
	m_Tag = "destructable";

	Damageable* damageable{ new Damageable{1} };
	RectCollider* collider{ new RectCollider{100,100,{}} };
	collider->m_Layer = Layers::NonEntity;
	collider->m_IsTrigger = true;

	DestructableObject* destructable{ new DestructableObject{} };

	AddComponent(destructable);
	AddComponent(damageable);
	AddComponent(collider);
}

void DestructableBarrel::Start()
{
	ParticleSystem* particleSystem{ new ParticleSystem
		{
			"VFX/particles_barrel.png",
			1,
			6,
			6,
			10,
			10,
			false,
			true
		}
	};

	ParticleSpawnRules spawnRules{ ParticleSpawnRules
		{
			20,
			4,
			6,
			false,
		}
	};

	std::function<void(Particle&)> particleSpawnRule
	{
		[this](Particle& particle)
		{
			float randAngle{ int(rand() % 361) * (float(M_PI) / 180)};
			Vector2f velocity{cosf(randAngle) * 100,sinf(randAngle) * 20 + 140};

			particle.Frame = int(rand() % 3);
			particle.Animate = false;
			particle.Velocity = velocity;
			particle.Gravity = Vector2f{ 0, -9.81f };
			particle.Mass = 20;
			particle.Rotation = float(rand() % 361);
			particle.RotationVelocity = 20;
			particle.LifeTime = 3;
		}
	};

	particleSystem->SetSpawnRule(spawnRules);
	particleSystem->SetSpawnRule(particleSpawnRule);

	GameObject* particleSystemObject{ new GameObject() };
	particleSystemObject->AddComponent(particleSystem);
	GetGameObjectManager()->AddObject(particleSystemObject);

	DestructableObject* destructableObject{ GetComponent<DestructableObject>() };
	destructableObject->Link(particleSystem);

	destructableObject->AddAudio(GetAudioManager()->GetAudioClip("SFX/breakable wall hit 1.mp3"));
	destructableObject->AddAudio(GetAudioManager()->GetAudioClip("SFX/breakable wall hit 2.mp3"));

	GameObject::Start();
}
