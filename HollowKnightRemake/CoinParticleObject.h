#pragma once
#include "GameObject.h"
#include "ParticleSystem.h"
#include "SoundEffect.h"

class CoinParticleObject final : public GameObject
{
public:
	CoinParticleObject();
	
	void Start() override;
	void Update(float elapsedTime) override;

	void Play(const Point2f& pos, int tinCoins = 10, int silverCoins = 0, bool dontDestroyOwn = false);

private:
	ParticleSystem* m_CoinParticleEffect;
	GameObject* m_CoinParticleParent;

	int m_RemaingTinPieces;
	int m_RemainingSilverPieces;
	
	bool m_Activated;

	std::vector<SoundEffect*> m_CoinHitSounds;
	std::vector<SoundEffect*> m_CoinCollectSounds;
};

