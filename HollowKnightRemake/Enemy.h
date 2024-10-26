#pragma once
#include "Damageable.h"
#include "Component.h"
#include <map>
#include "EnemySplashParticleEffect.h"
#include "CoinParticleObject.h"

struct AnimationSet 
{
	int State;
	Rectf Source;
	int Row;
	int StartFrame;
	int MaxFramesOnRow;
	float PlaySpeed;
	bool Loop;
	bool reverse;
};

class Texture;
class SpriteRenderer;
class GameObject;
class Enemy : public Component
{
public:
	Enemy(int health, bool useAnimation = true);
	virtual ~Enemy();

	virtual void Start() override;
	virtual void Update(float elpasedSec) override;
	virtual void Draw() const;

	virtual void OnTakeDamage(int damage);
	virtual void OnDead();

	void LoadAnimation(SpriteRenderer* renderer, std::map<int,AnimationSet> animations);
	void SetAnimationState(int state);

	virtual void OnCollisionEnter(Collider* collider);
	virtual void OnCollisionStay(Collider* collider);
	virtual void OnCollisionExit(Collider* collider);

protected:
	GameObject* GetPlayer();
	int GetHealth();

	bool isOverlappingWithPlayer(const Rectf& rect);
	bool isOverlappingWithPlayer(const Circlef& circle);
	bool isOverlappingWithPlayer(Collider* collider);

	virtual int GetTinValue();
	virtual int GetSilverValue();

private:
	void UpdateAnimation(float elapsedSec);
	Rectf GetSource();

	int m_CurrentState;
	int m_CurrentFrame;
	float m_AnimationTimer;
	bool m_UseAnimation;

	SpriteRenderer* m_Renderer;
	GameObject* m_PlayerGameObject;
	Collider* m_PlayerCollider;
	Damageable* m_DamageAble;

	std::map<int, AnimationSet> m_AllAnimations;

	SoundEffect* m_OnHitSound;
	SoundEffect* m_OnDeadSound;
	EnemySplashParticleEffect* m_SplashEffect;
	CoinParticleObject* m_CoinParticleObject;
};

