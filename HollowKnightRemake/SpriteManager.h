#pragma once
#include <vector>

class SpriteRenderer;
class SpriteManager final
{
public:
	SpriteManager();
	~SpriteManager();

	void Draw(const Point2f& cameraPos) const;
	
	void AddSprite(SpriteRenderer* renderer);
	void RemoveSprite(const SpriteRenderer* renderer);

	void Sort();

	void Deload();

private:
	void SortSprites();

	std::vector<SpriteRenderer*> m_AllSprites;
};

