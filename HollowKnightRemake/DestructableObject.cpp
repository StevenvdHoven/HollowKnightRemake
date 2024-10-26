#include "pch.h"
#include "DestructableObject.h"
#include "ParticleSystem.h"
#include "SpriteRenderer.h"
#include "Damageable.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "AudioManager.h"

DestructableObject::DestructableObject():
	m_Destroyed{false},
	m_Timer{ destructable_fadeawaytime },
	m_LinkedMapGameObject{ nullptr },
	m_System{ nullptr },
	m_Damageable{ nullptr },
	m_Collider{nullptr}
{
}

DestructableObject::~DestructableObject()
{
}

void DestructableObject::Start()
{
	m_Collider = GetComponent<Collider>();
	m_Collider->SetPosition(GetGameObject()->GetPosition());

	m_Damageable = GetComponent<Damageable>();
	if (m_Damageable != nullptr) 
	{
		m_Damageable->AddListenerTakeDamage(
			[this](int damage)
			{
				if (m_Destroyed)
				{
					return;
				}

				if (m_Damageable->GetHealth() <= 0 && !m_Destroyed)
				{
					m_Destroyed = true;

					m_Collider->SetEnabled(false);

 					GetGameObjectManager()->DeleteObject(m_LinkedMapGameObject);
					m_LinkedMapGameObject = nullptr;
				}

				if (m_SoundEffects.size() > 0)
				{
					int rndIndex{ int(rand() % m_SoundEffects.size()) };
					m_SoundEffects[rndIndex]->Play(0);
				}

				
				if (m_System != nullptr)
				{
					m_System->Play();
				}
				
			});
	}
}

void DestructableObject::Update(float elapsedSec)
{
	if (m_Destroyed)
	{
		m_Timer -= elapsedSec;
		if (m_Timer <= 0)
		{
			if (m_System != nullptr)
			{
				GetGameObjectManager()->DeleteObject(m_System->GetGameObject());
			}
			GetGameObjectManager()->DeleteObject(GetGameObject());
			return;
		}
	}
}

void DestructableObject::Link(GameObject* taggedGameObject)
{
	m_LinkedMapGameObject = taggedGameObject;
}

void DestructableObject::Link(ParticleSystem* particleSystem)
{
	m_System = particleSystem;
	m_System->GetGameObject()->SetPosition(GetGameObject()->GetPosition());
}

void DestructableObject::AddAudio(SoundEffect* sfx)
{
	m_SoundEffects.push_back(sfx);
}

void DestructableObject::AddAudio(const std::string& filepathSFX)
{
	SoundEffect* sfx{ GetAudioManager()->GetAudioClip(filepathSFX) };
	if (sfx != nullptr)
	{
		AddAudio(sfx);
	}
}
