#pragma once
#include "Component.h"

class Texture;
class ParticleSystem;
class SpriteRenderer : public Component
{
public:
	SpriteRenderer(int textureIndex,const Rectf& bounds, const Rectf& src, int order = 0, bool isBackground = false, bool isForeground = false);
	SpriteRenderer(Texture* texture, const Rectf& bounds, const Rectf& src, int order = 0, bool isBackground = false, bool isForeground = false);
	
	void Start() override;
	void Draw(const Point2f& cameraPos) const;
	void SetTexture(Texture* texture);
	void SetTexture(Texture* texture, const Rectf& bounds, const Rectf& src);
	void SetSource(const Rectf& src);
	void SetParticleSystem(ParticleSystem* particleSystem);
	void SetOrder(int order);
	void SetTint(const Color4f tint);
	void OnDelete() override;

	int GetOrder() const;
	bool GetIsBackground() const;
	bool GetIsForeground() const;

private:
	int m_TextureIndex;
	Texture* m_Texture;
	Rectf m_Bounds;
	Rectf m_Source;
	int m_Order;
	bool m_IsBackground;
	bool m_IsForeground;
	Color4f m_Tint;

	ParticleSystem* m_System;

};

