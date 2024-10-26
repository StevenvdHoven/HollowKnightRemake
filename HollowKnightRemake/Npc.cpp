#include "pch.h"
#include "Npc.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "SpriteRenderer.h"
#include "Collider.h"
#include "GUIManager.h"

Npc::Npc():
	m_CurrentState{0},
	m_CurrentFrame{0},
	m_AnimationTimer{0},
	m_TalkIndex{0},
	m_TalkTimer{npc_talk_rate}
{
}

void Npc::Start()
{
	m_PlayerGameObject = GetGameObjectManager()->GetObjectBasedOfTag("player");
	m_PlayerCollider = m_PlayerGameObject->GetComponent<Collider>();
	CreateTalkUI();
}

void Npc::Update(float elpasedSec)
{
	UpdateAnimation(elpasedSec);
	UpdateText(elpasedSec);
}

void Npc::Draw() const
{
}

void Npc::OnSpeachComplete()
{
}

void Npc::LoadAnimation(SpriteRenderer* renderer, std::map<int, AnimationSet> animations)
{
	m_Renderer = renderer;
	m_AllAnimations = animations;
	SetAnimationState(0);
}

void Npc::SetAnimationState(int state)
{
	if (state != m_CurrentState)
	{
		m_CurrentState = state;
		m_AnimationTimer = m_AllAnimations[m_CurrentState].PlaySpeed;
		m_CurrentFrame = !m_AllAnimations[m_CurrentState].reverse ? m_AllAnimations[m_CurrentState].StartFrame : m_AllAnimations[m_CurrentState].MaxFramesOnRow - 1;
	}
}

void Npc::Talk(const std::string& text)
{
	if (m_TextParent->IsHidden())
	{
		m_TextParent->SetHiding(false);
	}

	m_CurrentTextToBeLoaded = text;
	m_CurrentDisplayedText = "";
	m_TalkIndex = 0;
	m_Text->SetText(m_CurrentDisplayedText);
}

void Npc::CloseTalk()
{
	m_CurrentTextToBeLoaded = "";
	m_CurrentDisplayedText = "";
	m_TextParent->SetHiding(true);
}

GameObject* Npc::GetPlayer()
{
	return m_PlayerGameObject;
}

void Npc::CreateTalkUI()
{
	m_TextParent = new GUI{};
	m_TalkingBackground = new GUIShape
	{
		Rectf{1920 * 0.5f - 600,1080 * 0.8f - 150,1200,300},
		Color4f{0,0,0,0.3f}
	};

	m_Text = new GUIText
	{
		Point2f{1920 * 0.5f - 580,1080 * 0.8f + 100},
		"",
		GetGUIManager()->GetFont(trajan_pro_regular_24),
		white,
		TextAlign::Left
	};
	GetGUIManager()->AddGUI(m_TextParent);

	m_TalkingBackground->SetParent(m_TextParent);
	m_Text->SetParent(m_TextParent);

	m_TextParent->SetHiding(true);
}

void Npc::UpdateAnimation(float elapsedSec)
{
	m_AnimationTimer -= elapsedSec;
	if (m_AnimationTimer <= 0) {
		m_AnimationTimer = m_AllAnimations[m_CurrentState].PlaySpeed;

		if (!m_AllAnimations[m_CurrentState].reverse)
		{
			if (m_CurrentFrame >= m_AllAnimations[m_CurrentState].MaxFramesOnRow - 1)
			{
				if (m_AllAnimations[m_CurrentState].Loop) {
					m_CurrentFrame = m_AllAnimations[m_CurrentState].StartFrame;
				}
				else {
					m_CurrentFrame = m_AllAnimations[m_CurrentState].MaxFramesOnRow - 1;
				}
			}
			else {
				++m_CurrentFrame;
			}
		}
		else
		{
			if (m_CurrentFrame <= 0)
			{
				if (m_AllAnimations[m_CurrentState].Loop) {
					m_CurrentFrame = m_AllAnimations[m_CurrentState].MaxFramesOnRow - 1;
				}
				else {
					m_CurrentFrame = m_AllAnimations[m_CurrentState].StartFrame;
				}
			}
			else
			{
				--m_CurrentFrame;
			}

		}

	}
	m_Renderer->SetSource(GetSource());
}

void Npc::UpdateText(float elapsedSec)
{
	if (m_CurrentDisplayedText != m_CurrentTextToBeLoaded)
	{
		if (m_TalkTimer < 0)
		{
			m_TalkTimer = npc_talk_rate;

			int amount{ m_TalkIndex + 1 };
			const std::string content{ m_CurrentTextToBeLoaded };
			std::string newDisplayText(amount, '\0');
			for (int index{ 0 }; index < amount; ++index)
			{
				newDisplayText[index] = content[index];
			}
			m_CurrentDisplayedText = newDisplayText;
			m_Text->SetText(m_CurrentDisplayedText, 69);
			m_TalkIndex++;
		}
		else
		{
			m_TalkTimer -= elapsedSec;
		}

		if (m_CurrentDisplayedText == m_CurrentTextToBeLoaded)
		{
			OnSpeachComplete();
		}
	}
	
}

Rectf Npc::GetSource()
{
	Rectf source{ m_AllAnimations[m_CurrentState].Source };
	return Rectf
	{
		m_CurrentFrame * source.width,
		m_AllAnimations[m_CurrentState].Row * source.height + source.height,
		source.width,
		source.height
	};
}
