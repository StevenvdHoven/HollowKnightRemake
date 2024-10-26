#include "pch.h"
#include "Damageable.h"
#include <iostream>

Damageable::Damageable(int health):
	m_Health{health},
	m_MaxHealth{health}
{
	
}

void Damageable::TakeDamage(int damage)
{
	std::cout << GetGameObject()->m_Name << " Got hit!! Damage: " << damage << std::endl;

	int futureHealth = m_Health - damage;
	if (futureHealth <= 0 && m_Health != 0) {
		m_Health = 0;

		for (int index{ 0 }; index < m_OnDead.size(); ++index) {
			m_OnDead[index]();
		}
		
	}
	else {
		m_Health = futureHealth;
		
	}

	for (int index{ 0 }; index < m_OnTakeDamage.size(); ++index) {
		m_OnTakeDamage[index](damage);
	}
}

int Damageable::GetHealth()
{
	return m_Health;
}

int Damageable::GetMaxHealth()
{
	return m_MaxHealth;
}

void Damageable::SetHealth(int health)
{
	m_Health = health;
}

void Damageable::AddListenerTakeDamage(std::function<void(int)> takeDamage)
{
	m_OnTakeDamage.push_back(takeDamage);
}

void Damageable::AddListenerOnDead(std::function<void()> onDead)
{
	m_OnDead.push_back(onDead);
}
