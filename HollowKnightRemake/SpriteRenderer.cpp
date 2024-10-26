#include "pch.h"
#include "SpriteRenderer.h"
#include "utils.h"
#include "Texture.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "ParticleSystem.h"
#include <vector>


SpriteRenderer::SpriteRenderer(int textureIndex, const Rectf& bounds, const Rectf& src, int order, bool isBackground, bool isForeground):
	m_TextureIndex{ textureIndex },
	m_Texture{ nullptr },
	m_Bounds {bounds},
	m_Source{ src},
	m_Order{ order },
	m_IsBackground{ isBackground },
	m_IsForeground{ isForeground},
	m_Tint{1,1,1,1},
	m_System{ nullptr }
	
{
	
}

SpriteRenderer::SpriteRenderer(Texture* texture, const Rectf& bounds, const Rectf& src, int order, bool isBackground, bool isForeground):
	m_TextureIndex{ -1 },
	m_Texture{ texture },
	m_Bounds{ bounds },
	m_Source{ src },
	m_Order{ order },
	m_IsBackground{ isBackground },
	m_IsForeground{ isForeground },
	m_Tint{ 1,1,1,1 },
	m_System{ nullptr }
{
}


void SpriteRenderer::Start()
{
	if (m_Texture == nullptr) {
		m_Texture = GetTextureManager()->GetTexture(m_TextureIndex);
	}
	
	GetSpriteManager()->AddSprite(this);
}

void SpriteRenderer::Draw(const Point2f& cameraPos) const
{
	if (m_Texture == nullptr)
	{
		return;
	}
	Transform transform{ GetGameObject()->m_Transform };
	
	
	Rectf drawBounds = m_Source;
	drawBounds.left = -m_Source.width * 0.5f;
	drawBounds.bottom = -m_Source.height * 0.5f;

	Transform parallexTrans;
	if (m_IsBackground || m_IsForeground) {
		
		parallexTrans.Position = -(Vector2f(cameraPos) * std::abs(m_Order * 0.01f));
		
	}

	parallexTrans.ApplyTransformation();
	transform.ApplyTransformation();
	
	if (m_System != nullptr) 
	{
		std::vector<Particle> particles{ m_System->GetParticles() };

		for (int index{ 0 }; index < particles.size(); ++index)
		{
			if (!particles[index].IsLife)
			{
				continue;
			}

			Transform particleTrans;
			particleTrans.Position = Vector2f(particles[index].Position);
			particleTrans.Rotation = particles[index].Rotation;
			particleTrans.Scale = Vector2f(particles[index].Scale, particles[index].Scale);


			particleTrans.ApplyTransformation();
			m_Texture->Draw(drawBounds,Color4f{1,1,1,particles[index].Opacity}, particles[index].Source);
			particleTrans.ResetTransformation();
		}
	}
	else 
	{
		m_Texture->Draw(drawBounds,m_Tint, m_Source);
	}
	
	transform.ResetTransformation();
	parallexTrans.ResetTransformation();
	
}

void SpriteRenderer::SetTexture(Texture* texture)
{
	m_Texture = texture;
}

void SpriteRenderer::SetTexture(Texture* texture, const Rectf& bounds, const Rectf& src)
{
	m_Texture = texture;
	m_Bounds = bounds;
	m_Source = src;
}

void SpriteRenderer::SetSource(const Rectf& src)
{
	m_Source = src;
}

void SpriteRenderer::SetParticleSystem(ParticleSystem* particleSystem)
{
	m_System = particleSystem;
}

void SpriteRenderer::SetOrder(int order)
{
	m_Order = order;
	GetSpriteManager()->Sort();
}

void SpriteRenderer::SetTint(const Color4f tint)
{
	m_Tint = tint;
}

void SpriteRenderer::OnDelete()
{
	GetSpriteManager()->RemoveSprite(this);
}

int SpriteRenderer::GetOrder() const
{
	return m_Order;
}

bool SpriteRenderer::GetIsBackground() const
{
	return m_IsBackground;
}

bool SpriteRenderer::GetIsForeground() const
{
	return m_IsForeground;
}
