#include "pch.h"
#include "DestructableWeed.h"
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
#include "Curve.h"

DestructableWeed::DestructableWeed()
{
	m_Name = "Destructable_Weed";
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

void DestructableWeed::Start()
{
	ParticleSystem* particleSystem{ new ParticleSystem
		{
			"VFX/white_grass_new_particle.png",
			1,
			3,
			3,
			10,
			10,
			false,
			true
		}
	};

	ParticleSpawnRules spawnRules{ ParticleSpawnRules
		{
			20,
			12,
			20,
			false,
		}
	};

	std::function<void(Particle&)> particleSpawnRule
	{
		[this](Particle& particle)
		{
			float randAngle{ int(rand() % 361) * (float(M_PI) / 180)};
			Vector2f velocity{cosf(randAngle) * 200,sinf(randAngle) * 200};

			particle.Frame = int(rand() % 3);
			particle.Animate = false;
			particle.Speed = float(((rand() % 11 -10 + 1) + 10) * 0.1f);
			particle.Velocity = velocity;
			particle.Rotation = float(rand() % 361);
			particle.RotationVelocity = 20;
			particle.LifeTime = 2;
			particle.Scale = int(rand() % 10) * 0.1f + 0.5f;
			particle.Opacity = .8;

			particle.OpacityOverTime = curve_slope_down;
			particle.SpeedOverTime = Curve{ Vector2f{0,1}, Vector2f{.1,0}, Vector2f{.1,0}, Vector2f{1,0} };;
		}
	};

	particleSystem->SetSpawnRule(spawnRules);
	particleSystem->SetSpawnRule(particleSpawnRule);

	GameObject* particleSystemObject{ new GameObject() };
	particleSystemObject->AddComponent(particleSystem);
	GetGameObjectManager()->AddObject(particleSystemObject);

	DestructableObject* destructableObject{ GetComponent<DestructableObject>() }; 
	destructableObject->Link(particleSystem);

	destructableObject->AddAudio(GetAudioManager()->GetAudioClip("SFX/grass cut 1.mp3"));
	destructableObject->AddAudio(GetAudioManager()->GetAudioClip("SFX/grass cut 2.mp3"));
	destructableObject->AddAudio(GetAudioManager()->GetAudioClip("SFX/grass cut 3.mp3"));
	destructableObject->AddAudio(GetAudioManager()->GetAudioClip("SFX/grass cut 4.mp3"));
	destructableObject->AddAudio(GetAudioManager()->GetAudioClip("SFX/grass cut 5.mp3"));

	GameObject::Start();
}
