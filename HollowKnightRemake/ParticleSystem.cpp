#include "pch.h"
#include "ParticleSystem.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "PhysicsEngine.h"

ParticleSystem::ParticleSystem(
	Texture* texture,
	int nrCol,
	int nrRows,
	int nrFrames,
	float durration,
	float playSpeed,
	bool playOnAwake,
	bool loop) :
	m_FilePath{ "NAN" },
	m_Renderer{ nullptr },
	m_Texture{ texture },
	m_SourceRect{ Rectf{0,0,texture->GetWidth() / nrCol,texture->GetHeight() / nrRows} },
	m_NumberOfCollums{ nrCol },
	m_NumberOfRows{ nrRows },
	m_NumberOfFrames{ nrFrames },
	m_PlaySpeed{ playSpeed },
	m_Durration{ durration },
	m_CurrentTimer{ durration },
	m_PlayOnAwake{ playOnAwake },
	m_Playing{ false },
	m_Loop{ loop },
	m_ParticleStartRule{ nullptr },
	m_ParticleUpdateRule{ nullptr },
	m_ParticleDieRule{ nullptr },
	m_ParticleCollideRule{ nullptr },
	m_ParticleCollisionRule{ nullptr }
{
	m_SpawnRule = ParticleSpawnRules
	{
		1,
		1,
		1,
		false,
	};
}

ParticleSystem::ParticleSystem(const std::string& textfilepath, int nrCol, int nrRows, int nrFrames, float durration, float playSpeed, bool playOnAwake, bool loop) :
	m_FilePath{ textfilepath },
	m_Renderer{ nullptr },
	m_Texture{ nullptr },
	m_SourceRect{},
	m_NumberOfCollums{ nrCol },
	m_NumberOfRows{ nrRows },
	m_NumberOfFrames{ nrFrames },
	m_PlaySpeed{ playSpeed },
	m_Durration{ durration },
	m_CurrentTimer{ durration },
	m_PlayOnAwake{ playOnAwake },
	m_Playing{ false },
	m_Loop{ loop },
	m_ParticleStartRule{ nullptr },
	m_ParticleUpdateRule{ nullptr },
	m_ParticleDieRule{ nullptr },
	m_ParticleCollideRule{ nullptr },
	m_ParticleCollisionRule{ nullptr }
{
	m_SpawnRule = ParticleSpawnRules
	{
		1,
		1,
		1,
		false,
	};
}

void ParticleSystem::Play()
{
	Stop();

	m_Playing = true;
	m_CurrentTimer = m_Durration;
	m_SpawnTimer = -1;
	m_Renderer->SetEnabled(true);
	Spawn();
}

void ParticleSystem::Stop()
{
	m_Playing = false;
	m_Renderer->SetEnabled(false);
}

void ParticleSystem::Kill()
{
	m_Playing = false;
	m_Particles.clear();
	m_Renderer->SetEnabled(false);
}

void ParticleSystem::Start()
{
	if (m_Texture == nullptr) {
		m_Texture = GetTextureManager()->GetTexture(m_FilePath);
		m_SourceRect = Rectf{ 0,0,m_Texture->GetWidth() / m_NumberOfCollums,m_Texture->GetHeight() / m_NumberOfRows };
	}


	m_Renderer = GetComponent<SpriteRenderer>();
	if (m_Renderer == nullptr)
	{
		m_Renderer = new SpriteRenderer{ m_Texture,m_SourceRect,m_SourceRect, 1 };
		GetGameObject()->AddComponent(m_Renderer);

	}
	m_Renderer->SetParticleSystem(this);
	m_Renderer->SetOrder(10);

	if (m_PlayOnAwake)
	{
		Play();
	}
	else
	{
		Stop();
	}
}

void ParticleSystem::Update(float elpasedSec)
{
	if (!m_Playing)
	{
		return;
	}
	else
	{
		m_SpawnTimer -= elpasedSec;
		if (m_SpawnTimer <= 0)
		{
			Spawn();
			m_SpawnTimer = m_SpawnRule.SpawnRate;
		}
	}



	m_CurrentTimer -= elpasedSec;
	if (m_CurrentTimer < 0 && m_Playing)
	{
		Kill();
		return;
	}

	std::vector<Particle> aliveParticles;
	for (int index{ 0 }; index < m_Particles.size(); ++index)
	{
		m_Particles[index].LifeTime -= elpasedSec;
		const float percentile{ 1.f - (m_Particles[index].LifeTime / m_Particles[index].StartLifeTime) };
		if (m_Particles[index].LifeTime <= 0)
		{
			m_Particles[index].IsLife = false;
		}

		m_Particles[index].Timer -= elpasedSec;

		if (m_Particles[index].Timer <= 0 && m_Particles[index].Animate)
		{
			++m_Particles[index].Frame;
			m_Particles[index].Timer = m_PlaySpeed;
			if (m_Particles[index].Frame >= m_Particles[index].EndFrame)
			{
				if (m_Particles[index].Loop)
				{
					m_Particles[index].Frame = m_Particles[index].StartFrame;
				}
				else
				{
					m_Particles[index].Frame = m_Particles[index].EndFrame - 1;
				}
			}
		}

		m_Particles[index].Speed = m_Particles[index].SpeedOverTime.GetValue(percentile);
		m_Particles[index].Scale = m_Particles[index].ScaleOverTime.GetValue(percentile);
		m_Particles[index].Opacity = m_Particles[index].OpacityOverTime.GetValue(percentile);


		m_Particles[index].Source = GetSourceByFrame(m_Particles[index].Frame);
		m_Particles[index].Gravity = Vector2f::Lerp(m_Particles[index].StartGravity, m_Particles[index].TargetGravity, m_Particles[index].GravityOverTime.GetValue(percentile));
		m_Particles[index].Velocity += m_Particles[index].Mass * m_Particles[index].Gravity * elpasedSec * 2;
		m_Particles[index].Position += m_Particles[index].Velocity * m_Particles[index].Speed * elpasedSec;
		m_Particles[index].Rotation += m_Particles[index].RotationVelocity * elpasedSec;

		if ( m_Particles[index].IsCollider)
		{
			const Circlef circle{ m_Particles[index].Position + Vector2f(GetGameObject()->GetPosition()),m_Particles[index].Radius };
			std::vector<Collider*> results{ GetPhysicsEngine()->CircleCastAll(circle, { World }) };
			if (results.size() > 0)
			{
				if (m_ParticleCollideRule != nullptr)
				{
					m_ParticleCollideRule(m_Particles[index], results[0]);
				}
				
				if (m_ParticleCollisionRule != nullptr)
				{
					m_ParticleCollisionRule(m_Particles[index], results);
				}
			}
		}

		if (m_ParticleUpdateRule != nullptr)
		{
			m_ParticleUpdateRule(m_Particles[index]);
		}

		if (m_Particles[index].LifeTime > 0)
		{
			aliveParticles.push_back(m_Particles[index]);
		}
		else if (m_ParticleDieRule != nullptr)
		{
			m_ParticleDieRule(m_Particles[index]);
		}
	}

	m_Particles = std::move(aliveParticles);
}

void ParticleSystem::Draw() const
{
	Vector2f gameObjectPosition{ GetGameObject()->GetPosition() };

	for (int index{ 0 }; index < m_Particles.size(); ++index)
	{
		utils::SetColor(Color4f{ 0,0,1,1 });
		utils::DrawPoint(m_Particles[index].Position + gameObjectPosition, 5);

		utils::SetColor(Color4f{ 0,1,1,1 });
		utils::DrawLine(m_Particles[index].Position + gameObjectPosition, m_Particles[index].Position + gameObjectPosition + m_Particles[index].Velocity);

		if (m_Particles[index].IsCollider)
		{
			utils::SetColor(Color4f{ 1,0,0,1 });
			utils::DrawEllipse(Ellipsef{ m_Particles[index].Position + gameObjectPosition,m_Particles[index].Radius,m_Particles[index].Radius });
		}
	}
}

bool ParticleSystem::IsPlaying()
{
	return m_Playing;
}

bool ParticleSystem::IsLooping()
{
	return m_Loop;
}

Rectf ParticleSystem::GetSourceByFrame(int frame)
{
	Rectf src;
	int currentFrame{ 0 };
	for (int row{ 0 }; row < m_NumberOfRows; ++row) {
		for (int col{ 0 }; col < m_NumberOfCollums; ++col) {

			if (currentFrame == frame)
			{

				src = Rectf
				{
					col * m_SourceRect.width,
					row * m_SourceRect.height + m_SourceRect.height,
					m_SourceRect.width,
					m_SourceRect.height
				};
				return src;
			}
			++currentFrame;
		}
	}
	return src;

}

void ParticleSystem::SetSpawnRule(std::function<void(Particle& particle)> ruleOnSpawn)
{
	m_ParticleStartRule = ruleOnSpawn;
}

void ParticleSystem::SetSpawnRule(ParticleSpawnRules spawnRule)
{
	m_SpawnRule = spawnRule;
}

void ParticleSystem::SetUpdateRule(std::function<void(Particle&)> ruleOnUpdate)
{
	m_ParticleUpdateRule = ruleOnUpdate;
}

void ParticleSystem::SetDieRule(std::function<void(Particle&)> ruleOnDie)
{
	m_ParticleDieRule = ruleOnDie;
}

void ParticleSystem::SetOnCollideRule(std::function<void(Particle&, Collider*)> ruleOnCollision)
{
	m_ParticleCollideRule = ruleOnCollision;
}

void ParticleSystem::SetOnCollisionsRule(std::function<void(Particle&, std::vector<Collider*>)> ruleOnCollision)
{
	m_ParticleCollisionRule = ruleOnCollision;
}

std::vector<Particle> ParticleSystem::GetParticles()
{

	return m_Particles;
}

void ParticleSystem::SetOrder(int order)
{
	if (m_Renderer == nullptr)
	{
		m_Renderer = GetComponent<SpriteRenderer>();
		if (m_Renderer == nullptr)
		{
			m_Renderer = new SpriteRenderer{ m_Texture,m_SourceRect,m_SourceRect, 1 };
			GetGameObject()->AddComponent(m_Renderer);
		}
	}

	m_Renderer->SetOrder(order);
}

void ParticleSystem::Spawn()
{
	int rndAmount{ (int(rand()) % (m_SpawnRule.MaxAmountOfParticles - m_SpawnRule.MinAmountOfParticles + 1) + m_SpawnRule.MinAmountOfParticles) };

	if (!m_SpawnRule.Constant)
	{
		m_Particles = std::vector<Particle>(rndAmount);
		for (int index{ 0 }; index < m_Particles.size(); ++index)
		{
			m_Particles[index] = default_Particle;
			m_Particles[index].GravityOverTime = curve_default_0;
			m_Particles[index].Timer = m_PlaySpeed;
			m_Particles[index].StartFrame = 0;
			m_Particles[index].EndFrame = m_NumberOfFrames;

			if (m_ParticleStartRule != nullptr)
			{
				m_ParticleStartRule(m_Particles[index]);
			}
			m_Particles[index].StartLifeTime = m_Particles[index].LifeTime;
			m_Particles[index].SpeedOverTime.BaseValue = m_Particles[index].Speed;
			m_Particles[index].ScaleOverTime.BaseValue = m_Particles[index].Scale;
			m_Particles[index].OpacityOverTime.BaseValue = m_Particles[index].Opacity;
			m_Particles[index].StartGravity = m_Particles[index].Gravity;
		}
	}
	else
	{
		for (int index{ 0 }; index < rndAmount; ++index)
		{
			Particle currentParticle = default_Particle;
			currentParticle.GravityOverTime = curve_default_0;
			currentParticle.Timer = m_PlaySpeed;
			currentParticle.StartFrame = 0;
			currentParticle.EndFrame = m_NumberOfFrames;

			if (m_ParticleStartRule != nullptr)
			{
				m_ParticleStartRule(currentParticle);
			}
			currentParticle.StartLifeTime = currentParticle.LifeTime;
			currentParticle.SpeedOverTime.BaseValue = currentParticle.Speed;
			currentParticle.ScaleOverTime.BaseValue = currentParticle.Scale;
			currentParticle.OpacityOverTime.BaseValue = currentParticle.Opacity;
			currentParticle.StartGravity = currentParticle.Gravity;

			m_Particles.push_back(currentParticle);
		}
	}
}


