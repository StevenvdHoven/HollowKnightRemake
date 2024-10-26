#pragma once
#include "Component.h"

class KillZone final : public Component
{
public:
	KillZone();
	virtual ~KillZone();

	void Start() override;

	void OnTriggerEnter(Collider* collider) override;

private:
	bool m_Hit;
};

