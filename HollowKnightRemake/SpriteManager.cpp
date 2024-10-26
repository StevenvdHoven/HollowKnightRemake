#include "pch.h"
#include "SpriteManager.h"
#include "SpriteRenderer.h"
#include <algorithm>


SpriteManager::SpriteManager() :
	m_AllSprites{}
{
}

SpriteManager::~SpriteManager()
{
	m_AllSprites.clear();
}

void SpriteManager::Draw(const Point2f& cameraPos) const
{
	for (int index{ 0 }; index < m_AllSprites.size(); ++index)
	{
		if (m_AllSprites[index] == nullptr || !m_AllSprites[index]->IsEnabled()) {
			continue;
		}
		m_AllSprites[index]->Draw(cameraPos);
	}
}

void SpriteManager::AddSprite(SpriteRenderer* renderer)
{
	for (int index{ 0 }; index < m_AllSprites.size(); ++index) {
		if (m_AllSprites[index] == nullptr) {
			m_AllSprites[index] = renderer;
			SortSprites();
			return;
		}
	}
	m_AllSprites.push_back(renderer);
	SortSprites();
}

void SpriteManager::RemoveSprite(const SpriteRenderer* renderer)
{
	for (int index{ 0 }; index < m_AllSprites.size(); ++index) {
		if (m_AllSprites[index] == renderer) {
			m_AllSprites[index] = nullptr;
		}
	}
}

void SpriteManager::Sort()
{
	SortSprites();
}

void SpriteManager::Deload()
{
	for (int index{ 0 }; index < m_AllSprites.size(); ++index) {
		if (m_AllSprites[index] != nullptr) {
			delete m_AllSprites[index];
			m_AllSprites[index] = nullptr;
		}
	}
	m_AllSprites.clear();
}

void SpriteManager::SortSprites()
{
	//for (int first{ 0 }; first < m_AllSprites.size(); ++first) 
	//{
	//	if (m_AllSprites[first] == nullptr) {
	//		continue;
	//	}
	//	for (int second{ 0 }; second < m_AllSprites.size(); ++second) 
	//	{
	//		if (m_AllSprites[second] == nullptr) {
	//			continue;
	//		}

	//		if (m_AllSprites[first]->GetOrder() < m_AllSprites[second]->GetOrder()) 
	//		{
	//			SpriteRenderer* temp{ m_AllSprites[first] };
	//			m_AllSprites[first] = m_AllSprites[second];
	//			m_AllSprites[second] = temp;
	//		}
	//	}
	//}

	std::sort(m_AllSprites.begin(), m_AllSprites.end(), [](const SpriteRenderer* a, const SpriteRenderer* b)
		{
			if (a == nullptr) return false;
			if (b == nullptr) return true;

			if (a->GetIsBackground() || b->GetIsBackground())
			{
				if (a->GetIsBackground() && b->GetIsBackground()) {
					return a->GetOrder() < b->GetOrder();
				}
				else
				{
					return a->GetIsBackground();
				}
			}

			if (a->GetIsForeground() || b->GetIsForeground())
			{
				if (a->GetIsForeground() && b->GetIsForeground()) {
					return a->GetOrder() < b->GetOrder();
				}
				else
				{
					return !a->GetIsForeground();
				}
			}

			return a->GetOrder() < b->GetOrder();
		});
}


