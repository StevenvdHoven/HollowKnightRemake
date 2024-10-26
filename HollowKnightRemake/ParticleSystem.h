#pragma once
#include "Component.h"
#include "Texture.h"
#include "utils.h"
#include <functional>
#include <vector>
#include "Curve.h"

#define default_Particle Particle{ {},0,0,1,{},{},1,1,1,1,false,0,0,0,0,{},1,true,true,false,0.f,0}

struct ParticleSpawnRules
{
	float SpawnRate;
	int MinAmountOfParticles;
	int MaxAmountOfParticles;
	bool Constant;
};

struct Particle
{
	Point2f Position;
	float Rotation;
	float RotationVelocity;
	float Speed;
	Vector2f Velocity;
	Vector2f Gravity;
	float Mass;
	float Scale;
	
	float StartLifeTime;
	float LifeTime;
	bool Loop;
	int Frame;
	int StartFrame;
	int EndFrame;
	float Timer;
	Rectf Source;
	float Opacity;

	bool Animate;
	bool IsLife;

	bool IsCollider;
	float Radius;
	int Bounces;

	Vector2f StartGravity;
	Vector2f TargetGravity;

	Curve SpeedOverTime;
	Curve ScaleOverTime;
	Curve OpacityOverTime;
	Curve GravityOverTime;
};

class SpriteRenderer;
class ParticleSystem : public Component
{
public:
	ParticleSystem(
		Texture* texture,
		int nrCol, 
		int nrRows, 
		int nrFrames, 
		float durration,
		float playSpeed, 
		bool playOnAwake = true,
		bool loop = false);

	ParticleSystem(
		const std::string& textfilepath,
		int nrCol,
		int nrRows,
		int nrFrames,
		float durration,
		float playSpeed,
		bool playOnAwake = true,
		bool loop = false);

	void Play();
	void Stop();
	void Kill();
	void Start() override;
	void Update(float elpasedSec) override;
	void Draw() const override;

	bool IsPlaying();
	bool IsLooping();
	
	Rectf GetSourceByFrame(int frame);


	void SetSpawnRule(std::function<void(Particle&)> ruleOnSpawn);
	void SetSpawnRule(ParticleSpawnRules spawnRule);

	void SetUpdateRule(std::function<void(Particle&)> ruleOnUpdate);
	void SetDieRule(std::function<void(Particle&)> ruleOnDie);

	void SetOnCollideRule(std::function<void(Particle&, Collider*)> ruleOnCollision);
	void SetOnCollisionsRule(std::function<void(Particle&, std::vector<Collider*>)> ruleOnCollision);
	std::vector<Particle> GetParticles();

	void SetOrder(int order);

private:
	void Spawn();

	std::string m_FilePath;

	SpriteRenderer* m_Renderer;
	Texture* m_Texture;

	Rectf m_SourceRect;

	int m_NumberOfCollums;
	int m_NumberOfRows;
	int m_NumberOfFrames;

	float m_PlaySpeed;
	float m_Durration;
	float m_CurrentTimer;
	float m_SpawnTimer;
	
	bool m_PlayOnAwake;
	bool m_Playing;
	bool m_Loop;

	ParticleSpawnRules m_SpawnRule;
	std::function<void(Particle& particle)> m_ParticleStartRule;
	std::function<void(Particle& particle)> m_ParticleUpdateRule;
	std::function<void(Particle& particle)> m_ParticleDieRule;
	std::function<void(Particle& particle, Collider* collider)> m_ParticleCollideRule;
	std::function<void(Particle& particle, std::vector<Collider*> collider)> m_ParticleCollisionRule;


	std::vector<Particle> m_Particles;
};

