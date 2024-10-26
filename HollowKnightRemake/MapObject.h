#pragma once
#include <vector>
#include "json.hpp"
#include "utils.h"

using namespace nlohmann;

struct MapObjectData 
{
	int Index;
	int Order;
	std::string Tag;
	bool IsCollider;
	bool IsTrigger;
	bool IsBackground;
	bool IsForeground;

	int TextureIndex;

	Point2f Position;
	Vector2f Scale;
	float Rotation;

	Rectf SourceRect;
	Rectf Bounds;

	Color4f Tint;
};

class Texture;
class TextureManager;
class MapObject final
{
public:
	MapObject(int index, int textureIndex, const Point2f& position, const Rectf& src, TextureManager* textureManager);
	MapObject(const json& j, TextureManager* textureManager);
	MapObject(const MapObjectData& data, TextureManager* textureManager);

	void Draw(bool drawTexture = true, bool drawCollider = true) const;

	void SetPosition(const Point2f& pos);
	void SetScale(Vector2f scale);
	void SetRotation(float angle);
	void SetTextureIndex(int index);
	void SetBounds(const Rectf& bounds);
	void SetTag(std::string tag);
	void SetIsCollider(bool isCollider);
	void SetIsTrigger(bool isTrigger);
	void SetIsBackground(bool isBackground);
	void SetIsForeground(bool isForeground);
	void SetTint(const Color4f& color);
	bool PointInBounds(const Point2f& pos);

	Point2f GetPosition() const;
	float GetRotation() const;
	Vector2f GetScale() const;
	Rectf GetBounds() const;
	int GetTextureIndex() const;
	Rectf GetSourceRect() const;
	std::string GetTag() const;
	bool GetIsCollider() const;
	bool GetIsTrigger() const;
	bool GetIsBackground() const;
	bool GetIsForeground() const;
	Color4f GetTint() const;

	void ToJson(json& j);
	MapObjectData GetData();

	int m_Index;
	int m_Order;
private:
	std::string m_Tag;
	bool m_IsCollider;
	bool m_IsTrigger;
	bool m_IsBackground;
	bool m_IsForeground;

	int m_TextureIndex;
	Texture* m_Texture;

	Point2f m_Position;
	Vector2f m_Scale;
	float m_Rotation;

	Rectf m_SourceRect;
	Rectf m_Bounds;

	Color4f m_Tint;

	TextureManager* m_TextureManager;
};

