#include "pch.h"
#include "MapObject.h"
#include "Texture.h"
#include "Transform.h"
#include "utils.h"
#include "TextureManager.h"

MapObject::MapObject(int index, int textureIndex, const Point2f& position, const Rectf& src, TextureManager* textureManager) :
	m_Index{ index },
	m_Order{ 0 },
	m_Tag{},
	m_IsCollider{ true },
	m_IsTrigger{false},
	m_IsBackground{ false },
	m_IsForeground{ false },
	m_TextureIndex{ textureIndex },
	m_Texture{ nullptr },
	m_Position{ position },
	m_Scale{ 1,1 },
	m_Rotation{ 0 },
	m_SourceRect{src},
	m_Bounds{Rectf{position.x,position.y,src.width,src.height}},
	m_Tint{1,1,1,1},
	m_TextureManager{ textureManager }
{
	if (m_TextureIndex != -1) 
	{
		m_Texture = textureManager->GetTexture(m_TextureIndex);
		if (!m_Texture->IsCreationOk()) 
		{
			m_Texture = nullptr;
		}
	}
}

MapObject::MapObject(const json& j, TextureManager* textureManager):
	m_TextureManager{ textureManager }
{
	const json pos{ j["Position"].get<json>() };
	const json scale{ j["Scale"].get<json>() };
	const json src{ j["SourceRect"].get<json>() };
	const json bounds{ j["Bounds"].get<json>() };
	const json tint{ j["Color"].get<json>() };
	

	m_Index = j["Index"].get<int>();
	m_Order = j["Order"].get<int>();
	m_Tag = j["Tag"].get<std::string>();
	m_IsCollider = j["IsCollider"].get<bool>();
	m_IsTrigger = j["IsTrigger"].get<bool>();
	m_IsBackground = j["IsBackground"].get<bool>();
	m_IsForeground = j["IsForeground"].get<bool>();
	m_TextureIndex = j["TextureIndex"].get<int>();
	if (m_TextureIndex != -1) {
		m_Texture = m_TextureManager->GetTexture(m_TextureIndex);
		if (!m_Texture->IsCreationOk()) {
			m_Texture = nullptr;
		}
	}
	m_Position = Point2f(pos["X"].get<float>(), pos["Y"].get<float>());
	m_Scale = Vector2f(scale["X"].get<float>(),scale["Y"].get<float>());
	m_Rotation = j["Rotation"].get<float>();
	m_SourceRect = Rectf{
		src["Left"].get<float>(),
		src["Bottom"].get<float>(),
		src["Width"].get<float>(),
		src["Height"].get<float>(),
	};
	m_Bounds = Rectf{
		bounds["Left"].get<float>(),
		bounds["Bottom"].get<float>(),
		bounds["Width"].get<float>(),
		bounds["Height"].get<float>(),
	};

	m_Tint = Color4f{
		tint["r"].get<float>(),
		tint["g"].get<float>(),
		tint["b"].get<float>(),
		tint["a"].get<float>()
	};
}

MapObject::MapObject(const MapObjectData& data, TextureManager* textureManager):
	m_Index{data.Index},
	m_Order{data.Order},
	m_Tag{data.Tag},
	m_IsCollider{data.IsCollider},
	m_IsTrigger{data.IsTrigger},
	m_IsBackground{data.IsBackground},
	m_IsForeground{data.IsForeground},
	m_TextureIndex{data.TextureIndex},
	m_Texture{nullptr},
	m_Position{data.Position},
	m_Scale{data.Scale},
	m_Rotation{data.Rotation},
	m_SourceRect{data.SourceRect},
	m_Bounds{data.Bounds},
	m_Tint{data.Tint},
	m_TextureManager{textureManager}
{
	if (m_TextureIndex != -1)
	{
		m_Texture = textureManager->GetTexture(m_TextureIndex);
		if (!m_Texture->IsCreationOk())
		{
			m_Texture = nullptr;
		}
	}
}

void MapObject::Draw(bool drawTexture, bool drawCollider) const
{
	Transform transform;
	transform.Position = Vector2f(m_Position.x, m_Position.y);

	transform.Rotation = m_Rotation;
	transform.Scale = m_Scale;

	transform.ApplyTransformation();
	if (m_TextureIndex != -1 && drawTexture) 
	{

		m_Texture->Draw(Rectf{ -m_SourceRect.width * 0.5f,-m_SourceRect.height * 0.5f,m_SourceRect.width,m_SourceRect.height },m_Tint, m_SourceRect);
	}
	
	transform.ResetTransformation();
	
	if (!drawCollider) {
		if (!m_IsCollider) {
			return;
		}
	}

	Rectf rect{ -m_Bounds.width * 0.5f,-m_Bounds.height * 0.5f,m_Bounds.width,m_Bounds.height };
	rect = m_Bounds;
	rect.left = -m_Bounds.width * 0.5f;
	rect.bottom = -m_Bounds.height * 0.5f;
	std::vector<Point2f> poly{ utils::ApplyMatrix(transform,utils::GetPolygon(rect)) };
	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::DrawPolygon(poly);
}

void MapObject::SetPosition(const Point2f& pos)
{
	m_Position = pos;
	m_Bounds.left = pos.x;
	m_Bounds.bottom = pos.y;
}

void MapObject::SetScale(Vector2f scale)
{
	m_Scale = scale;
}

void MapObject::SetRotation(float angle)
{
	m_Rotation = angle;
}

void MapObject::SetTextureIndex(int index)
{
	m_TextureIndex = index;
	m_Texture = m_TextureManager->GetTexture(m_TextureIndex);
}

void MapObject::SetBounds(const Rectf& bounds)
{
	float widthDif{ bounds.width - m_Bounds.width };
	float heightDif{ bounds.height - m_Bounds.height };

	m_Bounds.left = m_Bounds.left + widthDif * 0.5f;
	m_Bounds.bottom = m_Bounds.bottom + heightDif * 0.5f;
	m_Bounds.width = bounds.width;
	m_Bounds.height = bounds.height;

	SetPosition(GetPosition());
}

void MapObject::SetTag(std::string tag)
{
	m_Tag = tag;
}

void MapObject::SetIsCollider(bool isCollider)
{
	m_IsCollider = isCollider;
}

void MapObject::SetIsTrigger(bool isTrigger)
{
	m_IsTrigger = isTrigger;
}

void MapObject::SetIsBackground(bool isBackground)
{
	m_IsBackground = isBackground;
}

void MapObject::SetIsForeground(bool isForeground)
{
	m_IsForeground = isForeground;
}

void MapObject::SetTint(const Color4f& color)
{
	m_Tint = color;
}


bool MapObject::PointInBounds(const Point2f& pos)
{
	Transform trans;
	trans.Position = Vector2f(m_Position);
	trans.Rotation = m_Rotation;
	trans.Scale = m_Scale;
	Rectf rect{ m_Bounds };
	rect.left = -m_Bounds.width * 0.5f;
	rect.bottom = -m_Bounds.height * 0.5f;

	std::vector<Point2f> poly{ utils::ApplyMatrix(trans,utils::GetPolygon(rect))};

	return utils::IsPointInPolygon(pos, poly);
}

Point2f MapObject::GetPosition() const
{
	return m_Position;
}

float MapObject::GetRotation() const
{
	return m_Rotation;
}

Vector2f MapObject::GetScale() const
{
	return m_Scale;
}

Rectf MapObject::GetBounds() const
{
	return m_Bounds;
}

int MapObject::GetTextureIndex() const
{
	return m_TextureIndex;
}

Rectf MapObject::GetSourceRect() const
{
	return m_SourceRect;
}

std::string MapObject::GetTag() const
{
	return m_Tag;
}

bool MapObject::GetIsCollider() const
{
	return m_IsCollider;
}

bool MapObject::GetIsTrigger() const
{
	return m_IsTrigger;
}

bool MapObject::GetIsBackground() const
{
	return m_IsBackground;
}

bool MapObject::GetIsForeground() const
{
	return m_IsForeground;
}

Color4f MapObject::GetTint() const
{
	return m_Tint;
}

void MapObject::ToJson(json& j)
{
	j.push_back(json::object({

			{"Index",m_Index},
			{"Order", m_Order},
			{"Tag", m_Tag},
			{"IsCollider", m_IsCollider},
			{"IsTrigger", m_IsTrigger},
			{"IsBackground",m_IsBackground},
			{"IsForeground",m_IsForeground},
			{"TextureIndex",m_TextureIndex},
			{"Position",json::object({{"X", m_Position.x},{"Y",m_Position.y}})},
			{"Scale", json::object({{"X",m_Scale.x},{"Y",m_Scale.y}}) },
			{"Rotation", m_Rotation},
			{"SourceRect",json::object({
				{"Left", m_SourceRect.left},
				{"Bottom",m_SourceRect.bottom},
				{"Width",m_SourceRect.width},
				{"Height",m_SourceRect.height}}),
			},
			{"Bounds",json::object({
				{"Left", m_Bounds.left},
				{"Bottom",m_Bounds.bottom},
				{"Width",m_Bounds.width},
				{"Height",m_Bounds.height}})
			},
			{"Color",json::object({
				{"r", m_Tint.r},
				{"g", m_Tint.g},
				{"b", m_Tint.b},
				{"a", m_Tint.a}})
			},

	}));
}

MapObjectData MapObject::GetData()
{
	return MapObjectData
	{
		m_Index,
		m_Order,
		m_Tag,
		m_IsCollider,
		m_IsTrigger,
		m_IsBackground,
		m_IsForeground,
		m_TextureIndex,
		m_Position,
		m_Scale,
		m_Rotation,
		m_SourceRect,
		m_Bounds,
		m_Tint
	};
}

