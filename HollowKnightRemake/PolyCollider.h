#pragma once
#include "Collider.h"
class PolyCollider : public Collider
{
public:
	PolyCollider(const std::vector<Point2f>& verticis, const Point2f& pos);

	bool CheckCollision(const Rectf& rect) override;
	bool CheckCollision(const Circlef& circle) override;
	bool CheckCollision(const std::vector<Point2f>& poly) override;
	bool CheckCollision(Collider* other) override;
	bool CheckCollision(Collider* other, const Point2f& futurePos) override;

	bool CheckWorldBounds(const Rectf& rect) override;
	bool CheckWorldBounds(const Rectf& rect, const Point2f& futurePos) override;
	Vector2f GetNormalFromBounds(const Rectf& rect) override;
	Vector2f GetNormalFromBounds(const Rectf& rect, const Point2f& futurePos) override;
	std::vector<Point2f> GetPolygon() override;

	float GetHeight() override;
	float GetWidth() override;

	void Draw() const override;
private:
	float CaculateWidth();
	std::vector<Point2f> GetWorldVerticis(const Point2f& position);
	
	
	float m_Width;
	std::vector<Point2f> m_Verticis;
	
};

