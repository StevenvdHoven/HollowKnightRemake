#pragma once
#include "Component.h"
#include "Enemy.h"
#include "GUI.h"
#include "GUIShape.h"
#include "GUIText.h"

#define npc_talk_rate .05f

enum Npc_01
{
	Idle,
	Talking,
};

class Collider;
class SpriteRenderer;
class Npc : public Component
{
public:
	Npc();

	virtual void Start() override;
	virtual void Update(float elpasedSec) override;
	virtual void Draw() const override;

	virtual void OnSpeachComplete();

	void LoadAnimation(SpriteRenderer* renderer, std::map<int, AnimationSet> animations);
	void SetAnimationState(int state);

	void Talk(const std::string& text);
	void CloseTalk();

protected:
	GameObject* GetPlayer();

private:
	void CreateTalkUI();
	void UpdateAnimation(float elapsedSec);
	void UpdateText(float elapsedSec);
	Rectf GetSource();

	int m_CurrentState;
	int m_CurrentFrame;
	float m_AnimationTimer;

	SpriteRenderer* m_Renderer;
	GameObject* m_PlayerGameObject;
	Collider* m_PlayerCollider;

	std::map<int, AnimationSet> m_AllAnimations;

	// Text UI
	float m_TalkTimer;
	int m_TalkIndex;

	std::string m_CurrentTextToBeLoaded;
	std::string m_CurrentDisplayedText;

	GUI* m_TextParent;
	GUIShape* m_TalkingBackground;
	GUIText* m_Text;



};

