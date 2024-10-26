#include "pch.h"
#include "GUIimage.h"
#include "Texture.h"

GUIimage::GUIimage(const Transform& transform, Texture* texture):
	m_Transform{ transform },
	m_Texture{texture},
	m_Tint{Color4f{1,1,1,1}},
	m_Rect{Rectf{0,0,texture->GetWidth(),texture->GetHeight()}},
	m_Source{m_Rect},
	m_Fill{1},
	m_FillType{LeftRight}
{
}

GUIimage::~GUIimage()
{
	m_Texture = nullptr;
}

void GUIimage::Draw()
{
	if (m_Texture == nullptr)
	{
		return;
	}

	m_Transform.ApplyTransformation();
	Rectf source{ GetSourceBasedOfFill() };
	Rectf drawBounds{ GetDrawRectBasedOfFill()};
	drawBounds.left += -m_Rect.width * .5f;
	drawBounds.bottom += -m_Rect.height * .5f;
	drawBounds.height = source.height;
	drawBounds.width = source.width;
	

	
	
	m_Texture->Draw(drawBounds, m_Tint, source);
	m_Transform.ResetTransformation();
}

bool GUIimage::CheckOverUI(const Point2f& mousepos)
{
	Point2f bottomLeft{ utils::ApplyMatrix(m_Transform,Point2f{m_Rect.left,m_Rect.bottom})};
	Point2f widthheight{ utils::ApplyMatrix(m_Transform,Point2f{m_Rect.width,m_Rect.height}) };
	Rectf bounds
	{
		bottomLeft.x,
		bottomLeft.y,
		widthheight.x,
		widthheight.y
	};

	return utils::IsPointInRect(mousepos,bounds);
}

void GUIimage::SetTransform(const Transform& transform)
{
	m_Transform = transform;
}

void GUIimage::SetTexture(Texture* texture)
{
	m_Texture = texture;

	m_Rect.width = texture->GetWidth();
	m_Rect.height = texture->GetHeight();
}

void GUIimage::SetTint(const Color4f& tint)
{
	m_Tint = tint;
}

void GUIimage::SetSource(const Rectf& source)
{
	m_Source = source;
}

void GUIimage::Fill(float fill)
{

	if (fill > 0.900999)
	{
		fill = 1;
	}
	else if (fill < 0.009)
	{
		fill = 0;
	}

	if (m_FillType == DownUp || m_FillType == RightLeft)
	{
		fill = 1 - fill;
	}
	

	m_Fill = fill;
}

void GUIimage::SetFillType(FillType type)
{
	m_FillType = type;
}

Rectf GUIimage::GetSourceBasedOfFill()
{

	Rectf source{ m_Source };

	switch (m_FillType)
	{
	case LeftRight:
		source.width = source.width * m_Fill;
		break;
	case RightLeft:
		source.left = source.left + source.width * (1.f - m_Fill);
		source.width = source.width * m_Fill;
		break;
	case DownUp:
		source.height = source.height - source.height * m_Fill;
		source.bottom = m_Rect.height;
		break;
	case UpDown:
		source.height = source.height - source.height * m_Fill;
		source.bottom = m_Rect.height;
		break;
	}

	return source;
}

Rectf GUIimage::GetDrawRectBasedOfFill()
{
	Rectf source{ m_Rect };

	switch (m_FillType)
	{
	case LeftRight:
		source.left = source.width * (1 - m_Fill);
		break;
	case RightLeft:
		source.left = source.left + source.width * (1.f - m_Fill);
		source.width = source.width * m_Fill;
		break;
	case DownUp:
		break;
	case UpDown:
		break;
	}

	return source;
}
