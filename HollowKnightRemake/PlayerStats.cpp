#include "pch.h"
#include "PlayerStats.h"
#include "TextureManager.h"
#include "GUIManager.h"
#include "PlayerDataManager.h"
#include "LevelManager.h"
#include "GameObjectManager.h"
#include "AudioManager.h"
#include "CameraManager.h"



PlayerStats::PlayerStats() :
	m_Focusing{ false },
	m_MapOpened{ false },
	m_AmountGeo{ 0 },
	m_Damageable{ nullptr },
	m_FocusEnergy{ 100 },
	m_FocusTimer{ focus_time },
	m_Healed{false},
	m_HealthPanelParent{ nullptr },
	m_HealthPower{ nullptr },
	m_HealthPowerFill{nullptr},
	m_Fill{1},
	m_FillMovement{0},
	m_CurrencyLogo{ nullptr },
	m_CurrencyText{ nullptr }
{
}

PlayerStats::~PlayerStats()
{
	GetGUIManager()->DeleteGUI(m_HealthPanelParent);
	GetGUIManager()->DeleteGUI(m_MapImage);
	m_HealthPower = nullptr;
	m_HealthPoints.clear();
	m_CurrencyLogo = nullptr;
	m_CurrencyText = nullptr;
	m_HealthPanelParent = nullptr;
}

void PlayerStats::Start()
{
	PlayerData data{ GetPlayerDataManager()->LoadPlayerData() };
	m_AmountGeo = data.Geo;
	m_RespawnPoint = data.KillRespawnPoint;
	m_HasMap = data.HasMap;

	m_Damageable = GetComponent<Damageable>();
	m_Damageable->SetHealth(data.Health);
	if (m_Damageable != nullptr)
	{
		m_Damageable->AddListenerTakeDamage([this](int damage)
			{
				UpdateUI();
			});

		m_Damageable->AddListenerOnDead([this]()
			{
				m_Damageable->SetHealth(5);
				GetLevelManager()->LoadLevel(m_RespawnPoint);
			});
	}

	CreateUI();
	UpdateUI();

	CreateCoinParticleSystem();
	CreateFocusLoopParticleSystem();

	m_FocusZoomCurve = Curve{ Vector2f{0,0}, Vector2f{1.5,1},Vector2f{0.5,1},Vector2f{1,0} };

	m_HealSoundEffect = GetAudioManager()->GetAudioClip("Player/focus health heal.mp3");
	m_ChargingSound = GetAudioManager()->GetAudioClip("Player/focus health charging.mp3");
}

void PlayerStats::Update(float elpasedSec)
{
	m_OnCoinPickUpParent->SetPosition(GetGameObject()->GetPosition());
	m_FocusLoopParent->SetPosition(GetGameObject()->GetPosition());
	m_FocusLoopParent2->SetPosition(GetGameObject()->GetPosition());
	m_FocusExplosionParent->SetPosition(GetGameObject()->GetPosition() - Vector2f{ 0,47.5f });
	m_FocusExplosionParent2->SetPosition(GetGameObject()->GetPosition());

	if (m_Focusing
		&& m_FocusTimer > 0)
	{
		m_FocusTimer -= elpasedSec;

		const float percent{ 1.f - (m_FocusTimer / focus_time) };
		const float curveValue{ m_FocusZoomCurve.GetValue(percent) };
		float zoom{ 1.f + 0.5f * curveValue };
		GetCameraManager()->SetZoom(zoom);


		if (percent >= 1.f)
		{
			m_FocusTimer = focus_time;
			m_Healed = false;
			m_Focusing = false;
		}
		else if (percent > 0.6f && !m_Healed)
		{
			m_FocusEnergy -= 10;
			m_Damageable->SetHealth(m_Damageable->GetHealth() + 1);
			UpdateUI();

			m_Healed = true;

			m_HealSoundEffect->Play(0);
			m_ChargingSound->Stop();

			m_FocusExplosionEffect->Play();
			m_FocusExplosionEffect2->Play();
			m_FocusLoopEffect->Stop();
			m_FocusLoopEffect2->Stop();
		}
		else
		{
			if (!m_FocusLoopEffect->IsPlaying())
			{
				m_FocusLoopEffect->Play();
				m_FocusLoopEffect2->Play();
			}
		}
	}

	if (m_FillMovement >= 1)
	{
		m_FillMovement = 0;
	}
	else
	{
		m_FillMovement += elpasedSec;
	}

	m_Fill = m_FocusEnergy / 100.f;
	m_HealthPowerFill->Fill(m_Fill + ((sinf(m_FillMovement * (M_PI * 2) * elpasedSec) * 0.01f)));
}

void PlayerStats::Draw() const
{
}

void PlayerStats::KeyDownEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_LSHIFT
		&& m_Damageable->GetHealth() != m_Damageable->GetMaxHealth()
		&& m_FocusEnergy >= 10)
	{
		m_Focusing = true;
		m_ChargingSound->Play(0);
	}
}

void PlayerStats::KeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (e.keysym.sym == SDLK_m && m_HasMap)
	{
		m_MapOpened = !m_MapOpened;
		m_MapImage->SetHiding(!m_MapOpened);
	}

	if (e.keysym.sym == SDLK_LSHIFT)
	{
		m_Focusing = false;
		m_FocusTimer = focus_time;
		m_ChargingSound->Stop();
		GetCameraManager()->SetZoom(1);
	}
}

void PlayerStats::Save()
{
	GetPlayerDataManager()->SavePlayerData(
		PlayerData
		{
			m_Damageable->GetHealth(),
			m_AmountGeo,
			0.f,
			GetLevelManager()->GetCurrentLevel(),
			m_HasMap,
			m_RespawnPoint
		});
}

void PlayerStats::GiveGeo(int amount)
{
	m_AmountGeo += amount;
	UpdateUI();

	m_OnCoinPickUpEffect->Play();
}

bool PlayerStats::CanBuy(int amount) const
{
	return m_AmountGeo >= amount;
}

void PlayerStats::Buy(int amount)
{
	m_AmountGeo -= amount;
	UpdateUI();
}

void PlayerStats::UnlockMap()
{
	m_HasMap = true;
}

void PlayerStats::SetRespawnPoint(const std::string& levelName)
{
	m_RespawnPoint = levelName;
}

void PlayerStats::GiveEnergy(float focusEnergy)
{
	float futureEnergy = m_FocusEnergy + focusEnergy;
	if (futureEnergy > 100)
	{
		m_FocusEnergy = 100.f;
	}
	else
	{
		m_FocusEnergy = futureEnergy;
	}

	UpdateUI();
}

void PlayerStats::UpdateUI()
{
	int currentHealth{ m_Damageable->GetHealth() };

	for (int index{ 0 }; index < m_Damageable->GetMaxHealth(); ++index)
	{
		m_HealthPoints[index]->SetTexture(m_HealthPointFullTexture);
	}

	for (int index{ currentHealth }; index < m_Damageable->GetMaxHealth(); ++index)
	{
		m_HealthPoints[index]->SetTexture(m_HealthPointEmptyTexture);
	}

	m_CurrencyText->SetText(std::to_string(m_AmountGeo));
}

void PlayerStats::CreateUI()
{
	m_HealthPanelParent = new GUI();

	Texture* healthui{ GetTextureManager()->GetTexture("UI/health_ui.png") };
	m_HealthPower = new GUIimage
	{
		Transform{Vector2f{healthui->GetWidth() * 0.5f + 50,1080 - (healthui->GetHeight() * 0.5f + 50)},0,Vector2f{1,1}},
		healthui,
	};

	m_HealthPower->SetParent(m_HealthPanelParent);

	m_HealthPowerFill = new GUIimage
	{
		Transform{Vector2f{healthui->GetWidth() * 0.5f + 50,1080 - (healthui->GetHeight() * 0.5f + 50)} + Vector2f{-37,-7},0,Vector2f{1,1} },
		GetTextureManager()->GetTexture("UI/health_ui_fill.png"),
	};

	m_HealthPowerFill->SetParent(m_HealthPanelParent);
	m_HealthPowerFill->SetFillType(DownUp);

	m_HealthPointFullTexture = GetTextureManager()->GetTexture("UI/healthpoint_full.png");
	m_HealthPointEmptyTexture = GetTextureManager()->GetTexture("UI/healthpoint_empty.png");

	float healthPointX{ healthui->GetWidth() * 0.5f + m_HealthPointEmptyTexture->GetWidth() + 50 };
	for (int index{ 0 }; index < m_Damageable->GetMaxHealth(); ++index)
	{
		GUIimage* guiImage
		{
			new GUIimage
			{
				Transform{Vector2f{healthPointX,1080 - (healthui->GetHeight() * 0.5f + 50)},0,Vector2f{1,1}},
				m_HealthPointFullTexture,
			}
		};

		guiImage->SetParent(m_HealthPanelParent);

		m_HealthPoints.push_back(guiImage);

		healthPointX += m_HealthPointFullTexture->GetWidth() + 15;
	}

	m_CurrencyLogo = new GUIimage
	{
		Transform{Vector2f{healthui->GetWidth() * 0.5f + m_HealthPointEmptyTexture->GetWidth() + 50,1080 - 170},0,Vector2f{1,1}},
		GetTextureManager()->GetTexture("UI/CurrencyLogo.png"),
	};

	m_CurrencyLogo->SetParent(m_HealthPanelParent);

	m_CurrencyText = new GUIText
	{
		Point2f{200,1080 - 190},
		"0",
		GetGUIManager()->GetFont(trajan_pro_regular_24),
		white,
		TextAlign::Left
	};

	m_CurrencyText->SetParent(m_HealthPanelParent);

	m_MapImage = new GUIimage
	{
		Transform{Vector2f{1920 * 0.5f,1080 * 0.5f},0,Vector2f{1,1}},
		GetTextureManager()->GetTexture("UI/Map.png")
	};
	m_MapImage->SetHiding(true);
	GetGUIManager()->AddGUI(m_MapImage);

	GetGUIManager()->AddGUI(m_HealthPanelParent);
}

void PlayerStats::CreateCoinParticleSystem()
{
	m_OnCoinPickUpEffect = new ParticleSystem
	{
		GetTextureManager()->GetTexture("VFX/CoinPickUp.png"),
		3,
		1,
		3,
		.15f,
		.5f,
		false
	};

	ParticleSpawnRules spawnRules
	{
		20,
		1,
		1,
		true
	};

	std::function<void(Particle&)> onSpawnRule
	{
		[](Particle& particle)
		{
			particle.Scale = 0.5f;
		}
	};

	m_OnCoinPickUpEffect->SetSpawnRule(spawnRules);
	m_OnCoinPickUpEffect->SetSpawnRule(onSpawnRule);
	m_OnCoinPickUpParent = new GameObject();
	m_OnCoinPickUpParent->AddComponent(m_OnCoinPickUpEffect);
	GetGameObjectManager()->AddObject(m_OnCoinPickUpParent);
}

void PlayerStats::CreateFocusLoopParticleSystem()
{
	m_FocusLoopEffect = new ParticleSystem
	{
		GetTextureManager()->GetTexture("VFX/ChargeParticles.png"),
		6,
		1,
		6,
		.6f,
		.1f,
		false,
		true
	};

	ParticleSpawnRules spawnRules
	{
		20,
		1,
		1,
		false
	};

	std::function<void(Particle&)> onSpawnRule
	{
		[](Particle& particle)
		{
			particle.Scale = 1;
			particle.Animate = true;
			particle.LifeTime = .6f;
		}
	};

	m_FocusLoopEffect->SetSpawnRule(spawnRules);
	m_FocusLoopEffect->SetSpawnRule(onSpawnRule);
	m_FocusLoopParent = new GameObject();
	m_FocusLoopParent->AddComponent(m_FocusLoopEffect);
	GetGameObjectManager()->AddObject(m_FocusLoopParent);

	m_FocusLoopEffect2 = new ParticleSystem
	{
		GetTextureManager()->GetTexture("VFX/ChargeParticles2.png"),
		3,
		1,
		3,
		.3f,
		.05f,
		false,
		true
	};

	ParticleSpawnRules spawnRules2
	{
		20,
		1,
		1,
		false
	};

	std::function<void(Particle&)> onSpawnRule2
	{
		[](Particle& particle)
		{
			particle.Scale = .7f;
			particle.Animate = true;
			particle.LifeTime = .6f;
		}
	};

	m_FocusLoopEffect2->SetSpawnRule(spawnRules2);
	m_FocusLoopEffect2->SetSpawnRule(onSpawnRule2);
	m_FocusLoopParent2 = new GameObject();
	m_FocusLoopParent2->AddComponent(m_FocusLoopEffect2);
	GetGameObjectManager()->AddObject(m_FocusLoopParent2);

	m_FocusExplosionEffect = new ParticleSystem
	{
		GetTextureManager()->GetTexture("VFX/White_particle.png"),
		1,
		1,
		1,
		.3f,
		.05f,
		false,
	};

	ParticleSpawnRules spawnRules3
	{
		20,
		20,
		30,
		false
	};

	std::function<void(Particle&)> onSpawnRule3
	{
		[](Particle& particle)
		{
			int randAngle{int(rand() % 361)};
			Vector2f velocity{cosf(randAngle) * 200,0};
			particle.Velocity = velocity;
			particle.LifeTime = 5;
			particle.Speed = int(10 + rand() % 6) * 0.1f;
			particle.Scale = 0.2f;
			particle.Mass = 40;

			particle.OpacityOverTime = curve_s_down_shape;
			particle.StartGravity = Vector2f{ 0,2 };
			particle.TargetGravity = Vector2f{ 0,9.81 };
			particle.GravityOverTime = curve_slope_up;
		}
	};

	m_FocusExplosionEffect->SetSpawnRule(spawnRules3);
	m_FocusExplosionEffect->SetSpawnRule(onSpawnRule3);
	m_FocusExplosionParent = new GameObject();
	m_FocusExplosionParent->AddComponent(m_FocusExplosionEffect);
	GetGameObjectManager()->AddObject(m_FocusExplosionParent);

	m_FocusExplosionEffect2 = new ParticleSystem
	{
		GetTextureManager()->GetTexture("VFX/ChargeExplosion.png"),
		6,
		1,
		6,
		.6f,
		.1f,
		false,
		false
	};

	ParticleSpawnRules spawnRules4
	{
		20,
		1,
		1,
		false
	};

	std::function<void(Particle&)> onSpawnRule4
	{
		[](Particle& particle)
		{
			particle.Scale = 2;
			particle.Animate = true;
			particle.LifeTime = .6f;

			particle.ScaleOverTime = Curve{ Vector2f{0,0},Vector2f{0.5,0},Vector2f{1,0.5},Vector2f{1,1} };
		}
	};

	m_FocusExplosionEffect2->SetSpawnRule(spawnRules4);
	m_FocusExplosionEffect2->SetSpawnRule(onSpawnRule4);
	m_FocusExplosionParent2 = new GameObject();
	m_FocusExplosionParent2->AddComponent(m_FocusExplosionEffect2);
	GetGameObjectManager()->AddObject(m_FocusExplosionParent2);
}

