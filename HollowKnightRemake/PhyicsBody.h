#pragma once
#include "Component.h"
#include "utils.h"
#include "Collider.h"



class PhyicsBody : public Component
{
	
public:
	PhyicsBody();
	PhyicsBody(float mass, float bounce, bool isStatic, int layer = 0);
	~PhyicsBody();

	void OnDelete() override;

	void Init() override;
	void Start() override;
	void FixedUpdate(float elapsedSec);
	void Simulate(Collider* other, float elpasedTime);
	void AddForce(Vector2f force,bool velocityChange = false);
	Point2f GetPerdictedPosition(float elpasedTime);
	Point2f GetPerdictedPosition(const Vector2f& velocity,float elpasedTime);
	void SetForces(std::vector<Vector2f> forces);
	bool IsStatic();

	void SetBodyOnBodyCollision(bool collision);
	bool GetBodyOnBodyCollision();

	void SubStepCollision(Collider* other,Vector2f normal, float elapsedTime, int count = 0);

	void OnCollisionEnter(Collider* collider) override;


	float m_ElpasedTime;

	float m_Mass;
	float m_Bounce;
	float m_gravity;

	Vector2f m_Velocity;

	Collider* m_Collider;

	bool m_IsColliding;

	bool m_IsStatic;
	bool m_BodyOnBodyCollision;

	std::vector<Vector2f> m_AppliedForces;

	Rectf WorldBounds;
	std::vector<PhyicsBody*> Bodies;



};

