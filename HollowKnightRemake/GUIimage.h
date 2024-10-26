#pragma once
#include "GUI.h"
#include "utils.h"

enum FillType
{
	LeftRight,
	RightLeft,
	DownUp,
	UpDown,
	FillCenter
};

class Texture;
class GUIimage final : public GUI
{
public:
	GUIimage(const Transform& transform, Texture* texture);
	virtual ~GUIimage();

	virtual void Draw();

	virtual bool CheckOverUI(const Point2f& mousepos);

	void SetTransform(const Transform& transform);
	void SetTexture(Texture* texture);
	void SetTint(const Color4f& tint);
	void SetSource(const Rectf& source);
	void Fill(float fill);
	void SetFillType(FillType type);

private:

	Rectf GetSourceBasedOfFill();
	Rectf GetDrawRectBasedOfFill();

	Transform m_Transform;
	Texture* m_Texture;
	Color4f m_Tint;

	Rectf m_Rect;
	Rectf m_Source;
	float m_Fill;

	FillType m_FillType;
	
};

