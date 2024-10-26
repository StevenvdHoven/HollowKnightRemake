#pragma once
#include "Npc.h"
#include <vector>
#include "GUI.h"
#include "GUIShape.h"
#include "GUIimage.h"
#include "Button.h"
#include "GUIText.h"
#include "SoundEffect.h"
#include "SoundStream.h"

#define npc_01_interaction_range 200
#define npc_01_interaction_sound_range 2000

enum NPC_01_Converstation_state
{
	FirstContact,
	OfferMap,
	RefusesToBuy,
	NotEnoughGeo,
	ListeningAfterNotBuying,
	AfterBuying,
	ListeningAfterBuyingMap
};

class Npc01 final : public Npc
{
public:
	Npc01();
	virtual ~Npc01() override;

	void Start() override;
	void Update(float elpasedSec) override;
	void Draw() const override;


	void KeyUpEvent(const SDL_KeyboardEvent& e) override;


	void OnSpeachComplete() override;
	void CheckNextState();
private:
	void StopInteracting();
	void StartTalking();
	void CreateUI();
	void LoadSounds();
	void UpdateActOnState();

	bool m_Interacting;
	bool m_SwitchTrack;
	
	std::vector<std::string> m_SpokenText;
	int m_CurrentTextIndex;
	NPC_01_Converstation_state m_CurrentState;

	GUI* m_OfferParentUI;
	GUIShape* m_OfferBackground;

	//SFXs
	std::vector<SoundEffect*> m_TalkingSounds;
	SoundStream* m_HummingLoopSound;
	SoundStream* m_ScribbelingLoopSound;
	SoundStream* m_MainSoundTrack;
	
};

