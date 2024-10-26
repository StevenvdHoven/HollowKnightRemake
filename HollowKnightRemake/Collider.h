#pragma once
#include "Component.h"
#include "utils.h"
#include <functional>

class PhyicsBody;
class Collider : public Component
{
public:
	Collider(const Point2f& pos);
	virtual ~Collider();

	void Init() override;
	void OnDelete() override;

	virtual bool CheckCollision(const Rectf& rect);
	virtual bool CheckCollision(const Circlef& circle);
	virtual bool CheckCollision(const std::vector<Point2f>& poly);
	virtual bool CheckCollision(Collider* other);
	virtual bool CheckCollision(Collider* other, const Point2f& futurePos);


	virtual bool CheckWorldBounds(const Rectf& rect);
	virtual bool CheckWorldBounds(const Rectf& rect, const Point2f& futurePos);
	virtual Vector2f GetNormalFromBounds(const Rectf& rect );
	virtual Vector2f GetNormalFromBounds(const Rectf& rect, const Point2f& futurePos);
	virtual std::vector<Point2f> GetPolygon();
	void SetPosition(const Point2f& pos);
	Point2f GetPosition();

	virtual float GetHeight();
	virtual float GetWidth();

	virtual void Draw() const;

	virtual void OnCollisionEnter(Collider* collider) override;
	virtual void OnCollisionStay(Collider* collider) override;
	virtual void OnCollisionExit(Collider* collider) override;

	virtual void OnTriggerEnter(Collider* collider) override;
	virtual void OnTriggerStay(Collider* collider) override;
	virtual void OnTriggerExit(Collider* collider) override;

	void ActivateEvent(Collider* collider);

	std::vector<Collider*> GetCollidingColliders();
	void AddCollidingCollider(Collider* collider);
	void RemoveCollidingCollider(Collider* collider);

	std::vector<Collider*> m_CollidingColliders;

	int m_Layer;
	bool m_IsTrigger;

	std::vector<std::function<void(Collider* collider)>> m_OnCollisions;

protected:
		Point2f m_Position;
};

