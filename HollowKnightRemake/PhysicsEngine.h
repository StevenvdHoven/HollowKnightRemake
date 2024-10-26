#pragma once
#include "utils.h"
#include <vector>

#define gravity 9.81f

enum Layers 
{
	World		= 0,
	Static		= 1,
	Dynamic		= 2,
	Entity		= 3,
	NonEntity	= 4,
};

class Collider;
class PhyicsBody;
class PhysicsEngine final
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	void Update(float elapsedSec);
	void CheckCollsionEventType(Collider* first, Collider* second,bool colliding, bool repeated = false);
	Collider* CheckCollision(Collider* collider);
	Collider* CheckCollision(Collider* collider, const Point2f& futurePos);

	void Add(Collider* collider);
	void Add(PhyicsBody* body);
	void Delete(Collider* collider);
	void Delete(PhyicsBody* body);

	bool Raycast(const Point2f& orgin, const Point2f& end, utils::HitInfo& info, Collider** collider, const std::vector<Layers> layers = { Layers::World }, Collider* self = nullptr);
	bool RayCastAll(const Point2f& orgin, const Point2f& end, std::vector<utils::HitInfo>& info, std::vector<Collider*>& colliders, const std::vector<Layers> layers, Collider* self = nullptr);
	
	bool BoxCast(const Rectf& rect, Collider** collider, const std::vector<Layers> layers = { Layers::World }, Collider* self = nullptr);
    std::vector<Collider*> BoxCastAll(const Rectf& rect, const std::vector<Layers> layers = { Layers::World }, Collider* self = nullptr);

	bool CircleCast(const Circlef& circle, Collider** collider, const std::vector<Layers> layers = { Layers::World }, Collider* self = nullptr);
	std::vector<Collider*> CircleCastAll(const Circlef& circle, const std::vector<Layers> layers = { Layers::World }, Collider* self = nullptr);

	bool ContainsLayer(Layers layer, std::vector<Layers> layers);
	void Draw();

	void Deload();

private:
	std::vector<Collider*> m_Colliders;
	std::vector<PhyicsBody*> m_Bodies;

};

