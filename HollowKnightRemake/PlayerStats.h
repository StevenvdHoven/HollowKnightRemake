#pragma once
#include "Component.h"
#include "GUIimage.h"
#include "GUIText.h"
#include "Damageable.h"
#include <vector>
#include "ParticleSystem.h"
#include "Curve.h"
#include "SoundEffect.h"

#define focus_time 1.2f


class PlayerStats final : public Component
{
public:
	PlayerStats();
	virtual ~PlayerStats();

	void Start() override;
	void Update(float elpasedSec) override;
	void Draw() const override;

	void KeyDownEvent(const SDL_KeyboardEvent& e) override;
	void KeyUpEvent(const SDL_KeyboardEvent& e) override;

	void Save();
	
	void GiveGeo(int amount);
	bool CanBuy(int amount) const;
	void Buy(int amount);
	
	void UnlockMap();
	void SetRespawnPoint(const std::string& levelName);
	void GiveEnergy(float focusEnergy);

private:
	void UpdateUI();
	void CreateUI();
	void CreateCoinParticleSystem();
	void CreateFocusLoopParticleSystem();

	bool m_Focusing;
	bool m_MapOpened;

	int m_AmountGeo;
	bool m_HasMap;

	std::string m_RespawnPoint;

	Damageable* m_Damageable;
	ParticleSystem* m_OnCoinPickUpEffect;
	GameObject* m_OnCoinPickUpParent;

	ParticleSystem* m_FocusLoopEffect;
	GameObject* m_FocusLoopParent;

	ParticleSystem* m_FocusLoopEffect2;
	GameObject* m_FocusLoopParent2;

	ParticleSystem* m_FocusExplosionEffect;
	GameObject* m_FocusExplosionParent;

	ParticleSystem* m_FocusExplosionEffect2;
	GameObject* m_FocusExplosionParent2;

	float m_FocusEnergy;
	float m_FocusTimer;
	bool m_Healed;
	Curve m_FocusZoomCurve;

	SoundEffect* m_HealSoundEffect;
	SoundEffect* m_ChargingSound;

	GUI* m_HealthPanelParent;
	GUIimage* m_HealthPower;
	GUIimage* m_HealthPowerFill;
	float m_Fill;
	float m_FillMovement;

	Texture* m_HealthPointFullTexture;
	Texture* m_HealthPointEmptyTexture;
	std::vector<GUIimage*> m_HealthPoints;

	GUIimage* m_CurrencyLogo;
	GUIText* m_CurrencyText;

	GUIimage* m_MapImage;
};

