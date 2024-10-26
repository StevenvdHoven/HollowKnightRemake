#include "pch.h"
#include "DestructableGeo.h"
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


DestructableGeo::DestructableGeo()
{
	m_Name = "Destructable_Geo";
	m_Tag = "destructable";

	Damageable* damageable{ new Damageable{3} };
	RectCollider* collider{ new RectCollider{100,100,{}} };
	collider->m_Layer = Layers::NonEntity;
	collider->m_IsTrigger = true;

	DestructableObject* destructable{ new DestructableObject{} };

	AddComponent(destructable);

	AddComponent(damageable);
	AddComponent(collider);
}

DestructableGeo::~DestructableGeo()
{
	GetGameObjectManager()->DeleteObject(m_CoinEffectParent);
}

void DestructableGeo::Start()
{
	m_CoinEffectParent = new CoinParticleObject{};
	GetGameObjectManager()->AddObject(m_CoinEffectParent);

	DestructableObject* destructable{ GetComponent<DestructableObject>() };
	destructable->AddAudio(GetAudioManager()->GetAudioClip("SFX/geo rock hit 1.mp3"));
	destructable->AddAudio(GetAudioManager()->GetAudioClip("SFX/geo rock hit 2.mp3"));
	destructable->AddAudio(GetAudioManager()->GetAudioClip("SFX/geo rock hit 3.mp3"));


	Damageable* damageable{ GetComponent<Damageable>() };
	damageable->AddListenerTakeDamage([this,damageable](int damage)
		{
			if (damageable->GetHealth() >= 0)
			{
				m_CoinEffectParent->Play(GetPosition(), 7, 0,true);
			}
		});

	GameObject::Start();
}
