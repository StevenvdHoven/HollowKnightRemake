  #pragma once
#include "Component.h"
#include <vector>
#include <functional>

class Damageable : public Component
{
public:
	Damageable(int health);

	
	virtual void TakeDamage(int damage);

	int GetHealth();
	int GetMaxHealth();
	void SetHealth(int health);

	void AddListenerTakeDamage(std::function<void(int)> takeDamage);
	void AddListenerOnDead(std::function<void()> onDead);

private:
	int m_Health;
	int m_MaxHealth;

	std::vector<std::function<void(int)>> m_OnTakeDamage;
	std::vector<std::function<void()>> m_OnDead;
};

