#include "pch.h"
#include "CoinParticleObject.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "Collider.h"
#include "PlayerStats.h"
#include "PhysicsEngine.h"
#include "AudioManager.h"

CoinParticleObject::CoinParticleObject():
	m_CoinParticleEffect{ nullptr },
	m_CoinParticleParent{ nullptr },
	m_RemaingTinPieces{0},
	m_RemainingSilverPieces{0},
	m_Activated{false}
{
}

void CoinParticleObject::Start()
{
	GameObject::Start();

	m_CoinHitSounds.push_back(GetAudioManager()->GetAudioClip("SFX/geo rock hit 1.mp3"));
	m_CoinHitSounds.push_back(GetAudioManager()->GetAudioClip("SFX/geo rock hit 2.mp3"));
	m_CoinHitSounds.push_back(GetAudioManager()->GetAudioClip("SFX/geo rock hit 3.mp3"));

	m_CoinCollectSounds.push_back(GetAudioManager()->GetAudioClip("SFX/geo small collect 1.mp3"));
	m_CoinCollectSounds.push_back(GetAudioManager()->GetAudioClip("SFX/geo small collect 1-2.mp3"));
	m_CoinCollectSounds.push_back(GetAudioManager()->GetAudioClip("SFX/geo small collect 2.mp3"));
	m_CoinCollectSounds.push_back(GetAudioManager()->GetAudioClip("SFX/geo small collect 2-2.mp3"));

	m_CoinParticleEffect = new ParticleSystem
	{
			GetTextureManager()->GetTexture("VFX/Currnency.png"),
			6,
			4,
			24,
			20,
			0.2f,
			false,
	};

	std::function<void(Particle& particle)> onSpawnRules
	{
		[this](Particle& particle)
		{
			if (m_RemainingSilverPieces > 0)
			{
				particle.StartFrame = 12;
				particle.Frame = 12;
				particle.EndFrame = 23;
				--m_RemainingSilverPieces;
				
			}
			else
			{
				particle.StartFrame = 0;
				particle.EndFrame = 11;
			}

			particle.IsCollider = true;
			particle.Radius = 15;
			particle.Bounces = 2;
			particle.Loop;

			particle.LifeTime = 20;
			particle.Mass = 30;
			particle.Animate = true;
			particle.Gravity = Vector2f{ 0,-9.81f };

			float randAngle{float(rand() % 361)};
			Vector2f velocity{cosf(randAngle) * 200,sinf(randAngle) * 200 + 100};
			particle.Velocity = velocity;
		}
	};

	std::function<void(Particle&, std::vector<Collider*>)> onCollsionRules
	{
		[this](Particle& particle, std::vector<Collider*> colliders)
		{
			for (int index{0}; index < colliders.size(); ++index)
			{
				if (colliders[index]->GetGameObject()->m_Tag == "player")
				{
					PlayerStats* stats{ colliders[index]->GetComponent<PlayerStats>()};
					if (stats != nullptr && particle.LifeTime > 0 && particle.IsCollider)
					{
						stats->GiveGeo(particle.StartFrame == 0 ? 1 : 5);
						particle.LifeTime = -1;
						particle.IsCollider = false;

						int randSfx{int( rand() % m_CoinCollectSounds.size())};
						m_CoinCollectSounds[randSfx]->SetVolume(80);
						m_CoinCollectSounds[randSfx]->Play(0);
						
						return;
					}
				}
			}

			Collider* collider = colliders[0];

			if (collider->m_IsTrigger || collider->GetGameObject()->m_Tag == "enemy" || collider->m_Layer == Layers::NonEntity)
			{
				return;
			}

			
			if(particle.Mass != 0)
			{
				int randSfx{int( rand() % m_CoinHitSounds.size() )};
				m_CoinHitSounds[randSfx]->SetVolume(40);
				m_CoinHitSounds[randSfx]->Play(0);

				if (particle.Bounces > 0)
				{
					utils::HitInfo info;
					if(utils::Raycast(collider->GetPolygon(),  m_CoinParticleParent->GetPosition() + Vector2f(particle.Position) ,collider->GetPosition(),info))
					{
						particle.Velocity = particle.Velocity.Reflect(info.normal) * 0.25f;
						particle.Position = particle.Position + (particle.Velocity/ particle.Velocity.Length()) * 2;
						if (info.normal == Vector2f{ 0,1 })
						{
							particle.Bounces = particle.Bounces - 1;
						}
					}

					
				}
				else
				{
					particle.Velocity = Vector2f{};
					particle.Gravity = Vector2f{};
					particle.Speed = 0;
					particle.Mass = 0;
					particle.Frame = particle.StartFrame;
					particle.Animate = false;
				}
			}
		}
	};

	m_CoinParticleEffect->SetOnCollisionsRule(onCollsionRules);

	m_CoinParticleParent = new GameObject();
	m_CoinParticleParent->AddComponent(m_CoinParticleEffect);

	m_CoinParticleEffect->SetSpawnRule(onSpawnRules);

	GetGameObjectManager()->AddObject(m_CoinParticleParent);
}

void CoinParticleObject::Update(float elapsedTime)
{
	GameObject::Update(elapsedTime);

	if (!m_Activated)
	{
		return;
	}

	std::vector<Particle> particles{ m_CoinParticleEffect->GetParticles() };

	if (particles.size() == 0)
	{
		GetGameObjectManager()->DeleteObject(m_CoinParticleParent);
		GetGameObjectManager()->DeleteObject(this);
		return;
	}
}

void CoinParticleObject::Play(const Point2f& pos,int tinCoins,int silverCoins, bool dontDestroyOwn)
{
	ParticleSpawnRules spawnRules
	{
		20,
		tinCoins + silverCoins,
		tinCoins + silverCoins,
		true
	};

	m_RemaingTinPieces = tinCoins;
	m_RemainingSilverPieces = silverCoins;

	m_CoinParticleEffect->SetSpawnRule(spawnRules);
	m_CoinParticleParent->SetPosition(pos);
	m_CoinParticleEffect->Play();

	if (!dontDestroyOwn)
	{
		m_Activated = true;
	}
	
}
