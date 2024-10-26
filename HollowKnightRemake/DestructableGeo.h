#pragma once
#include "GameObject.h"
#include "CoinParticleObject.h"

class DestructableGeo final : public GameObject
{
public:
	DestructableGeo();
	virtual ~DestructableGeo() override;

	void Start() override;

private:
	CoinParticleObject* m_CoinEffectParent;
};

