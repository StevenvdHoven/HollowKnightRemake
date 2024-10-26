#include "pch.h"
#include "Cornifer.h"
#include "Npc01.h"
#include "SpriteRenderer.h"

Cornifer::Cornifer()
{
	m_Name = "Cornifer";
	m_Tag = "npc";

	Npc01* npcBehavior{ new Npc01{} };
	SpriteRenderer* render{ new SpriteRenderer{-1,{},{}} };

	AddComponent(npcBehavior);
	AddComponent(render);
}
