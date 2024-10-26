#include "pch.h"
#include "Npc01.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "GUIManager.h"
#include "PlayerStats.h"
#include "AudioManager.h"
#include "PlayerMovement.h"
#include "Player.h"

Npc01::Npc01():
	m_Interacting{false},
	m_CurrentTextIndex{0},
	m_CurrentState{FirstContact},
	m_SwitchTrack{true}
{
}

Npc01::~Npc01()
{
	GetGUIManager()->DeleteGUI(m_OfferParentUI);
}

void Npc01::Start()
{
	Npc::Start();

	m_SpokenText = std::vector<std::string>
	{
		"Hmm? Ah, hello there. Come down to explore these beautiful old ruins? Don't mind me. I've a fondness for exploring myself.",
		"Getting lost and finding your way again is a pleasure like no other. We're exquisitely lucky, you and I.",
		"I'm a cartographer by trade, and I'm working on mapping this area right now. Would you like to buy a copy of my work so far?"
	};

	Texture* texture{ GetTextureManager()->GetTexture("Characters/NPC_01.png") };
	SpriteRenderer* renderer{ GetComponent<SpriteRenderer>() };
	renderer->SetTexture(texture);

	const float playRate{ 1.f / 8.f };

	Rectf source{ 0,0,texture->GetWidth() / 3, texture->GetHeight() / 10 };
	std::map<int, AnimationSet> animations
	{
		{
			Idle,
			AnimationSet
			{
				Idle,
				source,
				2,
				0,
				3,
				playRate,
				true,
				false
			}
		},
		{
			Talking,
			AnimationSet
			{
				Talking,
				source,
				8,
				0,
				3,
				playRate,
				true,
				false,
			}
		}
	};

	LoadAnimation(renderer, animations);

	CreateUI();
	LoadSounds();
}

void Npc01::Update(float elpasedSec)
{
	Npc::Update(elpasedSec);

	if (m_Interacting)
	{
		SetAnimationState(Talking);
	}
	else
	{
		SetAnimationState(Idle);
		float distance{ utils::GetDistance(GetPlayer()->GetPosition(),GetGameObject()->GetPosition()) };
		if (distance < npc_01_interaction_sound_range)
		{
			float percentage{ 1 - (distance / npc_01_interaction_sound_range) };

			SoundStream::SetVolume(100 * percentage);
			if (!m_SwitchTrack)
			{
				m_SwitchTrack = true;
				m_HummingLoopSound->Play(true);
			}
		}
		else
		{
			if (m_SwitchTrack)
			{
				m_SwitchTrack = false;
				m_MainSoundTrack->Play(true);
				SoundStream::SetVolume(100);
			}
		}

	}
}

void Npc01::Draw() const
{
	Npc::Draw();

	const Ellipsef ellipse{ GetGameObject()->GetPosition(),npc_01_interaction_range ,npc_01_interaction_range };
	utils::SetColor(Color4f{ 0,0,1,1 });
	utils::DrawEllipse(ellipse);
}

void Npc01::KeyUpEvent(const SDL_KeyboardEvent& e)
{
	float distance{ utils::GetDistance(GetPlayer()->GetPosition(),GetGameObject()->GetPosition())};
	if (distance > npc_01_interaction_range)
	{
		return;
	}

	if (e.keysym.sym == SDLK_e)
	{
		if (!m_Interacting)
		{
			m_Interacting = true;
			Player* player{ (Player*)GetPlayer() };
			player->StopInput();

			SoundStream::Pause();

			StartTalking();
		}
		else
		{
			++m_CurrentTextIndex;
			if (m_CurrentTextIndex < m_SpokenText.size())
			{
				Talk(m_SpokenText[m_CurrentTextIndex]);
				int randIndex{ int(rand() % m_TalkingSounds.size()) };
				m_TalkingSounds[randIndex]->Play(0);
			}
			else 
			{
				CheckNextState();
			}
			
		}
	}
}

void Npc01::OnSpeachComplete()
{
	if (m_CurrentTextIndex > m_SpokenText.size())
	{
		CheckNextState();
	}
}

void Npc01::CheckNextState()
{
	switch (m_CurrentState)
	{
	case FirstContact:
		m_CurrentState = OfferMap;
		break;
	case RefusesToBuy:
		m_CurrentState = ListeningAfterNotBuying;
		StopInteracting();
		break;
	case NotEnoughGeo:
		m_CurrentState = ListeningAfterNotBuying;
		StopInteracting();
		break;
	case ListeningAfterNotBuying:
		m_CurrentState = OfferMap;
		break;
	case AfterBuying:
		m_CurrentState = ListeningAfterBuyingMap;
		StopInteracting();
		break;
	case ListeningAfterBuyingMap:
		StopInteracting();
		break;
	default:
		break;
	}

	UpdateActOnState();
}

void Npc01::StopInteracting()
{
	CloseTalk();
	m_Interacting = false;

	Player* player{ (Player*)GetPlayer() };
	player->ResumeInput();

	SoundStream::Resume();
}

void Npc01::StartTalking()
{
	m_CurrentTextIndex = 0;
	Talk(m_SpokenText[m_CurrentTextIndex]);

	int randIndex{ int(rand() % m_TalkingSounds.size()) };
	m_TalkingSounds[randIndex]->Play(0);
}

void Npc01::CreateUI()
{
	m_OfferParentUI = new GUI{};
	GetGUIManager()->AddGUI(m_OfferParentUI);

	m_OfferBackground = new GUIShape
	{
		Rectf{1920 * 0.5f - 550,1080 * 0.7f - 150,1100,300},
		Color4f{0.1f,0.1f,0.1f,0.3f}
	};
	m_OfferBackground->SetParent(m_OfferParentUI);

	GUIText* offerText{ new GUIText
		{
			Point2f{1920 * 0.5f,1080 * 0.7f + 50},
			"Purchase a map of the area?",
			GetGUIManager()->GetFont(trajan_pro_regular_40),
			white
		}
	};
	offerText->SetParent(m_OfferParentUI);

	GUIText* priceText{ new GUIText
		{
			Point2f{1920 * 0.5f - 10,1080 * 0.7f - 20},
			"30",
			GetGUIManager()->GetFont(trajan_pro_regular_40),
			white
		}
	};
	priceText->SetParent(m_OfferParentUI);

	GUIimage* geoLogo{ new GUIimage
		{
			Transform{Vector2f{1920 * 0.5f + 50,1080 * 0.7f - 10},0,Vector2f{1,1}},
			GetTextureManager()->GetTexture("UI/CurrencyLogo.png"),
		}
	};
	geoLogo->SetParent(m_OfferParentUI);

	std::function<void()> onBuyClick
	{
		[this]()
		{
			PlayerStats* stats{ GetPlayer()->GetComponent<PlayerStats>() };
			if (stats != nullptr)
			{
				if (stats->CanBuy(30))
				{
					stats->Buy(30);
					stats->UnlockMap();
					m_CurrentState = AfterBuying;
					UpdateActOnState();
				}
				else
				{
					m_CurrentState = NotEnoughGeo;
					UpdateActOnState();
				}
			}
			m_OfferParentUI->SetHiding(true);
		}
	};

	Button* buyButton{ new Button
		{
			Rectf{1920 * 0.5f - 300,1080 * 0.7f - 150,300,100},
			Color4f{0,0,0,0},
			white,
			"Yes",
			GetGUIManager()->GetFont(trajan_pro_regular_36),
			ButtonTypes::Up,
			onBuyClick
		}
	};
	buyButton->SetParent(m_OfferParentUI);

	std::function<void()> onRefuseBuyClick
	{
		[this]()
		{
			m_CurrentState = RefusesToBuy;
			UpdateActOnState();
			m_OfferParentUI->SetHiding(true);
		}
	};

	Button* refuseButton{ new Button
		{
			Rectf{1920 * 0.5f + 50,1080 * 0.7f - 150,300,100},
			Color4f{0,0,0,0},
			white,
			"No",
			GetGUIManager()->GetFont(trajan_pro_regular_36),
			ButtonTypes::Up,
			onRefuseBuyClick
		}
	};
	refuseButton->SetParent(m_OfferParentUI);

	m_OfferParentUI->SetHiding(true);
}

void Npc01::LoadSounds()
{
	m_TalkingSounds = std::vector<SoundEffect*>
	{
		GetAudioManager()->GetAudioClip("Cornifer/Cornifer 01.mp3"),
		GetAudioManager()->GetAudioClip("Cornifer/Cornifer 02.mp3"),
		GetAudioManager()->GetAudioClip("Cornifer/Cornifer 03.mp3"),
		GetAudioManager()->GetAudioClip("Cornifer/Cornifer 05.mp3"),
		GetAudioManager()->GetAudioClip("Cornifer/Cornifer 06.mp3"),
		GetAudioManager()->GetAudioClip("Cornifer/Cornifer 07.mp3"),
		GetAudioManager()->GetAudioClip("Cornifer/Cornifer 08.mp3"),
	};

	m_HummingLoopSound = GetAudioManager()->GetAudioStream("Cornifer/Cornifer Hum.mp3");

	m_HummingLoopSound->Play(true);

	m_MainSoundTrack = GetAudioManager()->GetAudioStream("SoundTrack/S19 Crossroads Main.mp3");
}

void Npc01::UpdateActOnState()
{
	switch (m_CurrentState)
	{
	case OfferMap:
		m_OfferParentUI->SetHiding(false);
		CloseTalk();
		break;
	case RefusesToBuy:
		m_SpokenText = std::vector<std::string>
		{
			"Not interested, then? Yes, quite understandable. Exploring without a map, finding your own way through unknown caverns... All very nourishing for the soul.Perhaps we'll meet again on our travels."
		};
		StartTalking();
		break;
	case NotEnoughGeo:
		m_SpokenText = std::vector<std::string>
		{
			"You don't have enough Geo? Ah, I understand. I'd give you the map as a gift, but I don't think my wife would be very happy if I did. All our food is made of Geo, as they say."
		};
		StartTalking();
		break;
	case ListeningAfterNotBuying:
		m_SpokenText = std::vector<std::string>
		{
			"Hmm? Have you changed your mind about buying a map?"
		};
		break;
	case AfterBuying:
		m_SpokenText = std::vector<std::string>
		{
			"A map can be a useful thing, but it alone won't show you where you are. If you've not the head for directions, I suggest purchasing a compass from my wife, Iselda. She's just now opening our new map shop in Dirtmouth, selling all sorts of useful things to wanderers like yourself. She'll even sell some of my old maps from time to time.I pop back to see her whenever I finish mapping an area.She's always so excited to see me."
		};
		StartTalking();
		break;
	case ListeningAfterBuyingMap:
		m_SpokenText = std::vector<std::string>
		{
			"Oh, by the way, I haven't introduced myself have I? Yes, I should apologise for that. When you spend a lot of time by yourself, with just your own thoughts, you forget the niceties of conversation. My name is Cornifer, and I've always loved exploring the world. Why, when I was first hatched I wandered off immediately, leaving my brothers and sisters and poor mother behind! That's why as soon as I could I moved to Dirtmouth with my wife. A huge, unending Kingdom to explore right on our doorstep. Who could resist? Anyway, I'll let you return to your travels. With a little luck, we'll meet again.Be safe and farewell!"
		};
		break;
	default:
		break;
	}
}
