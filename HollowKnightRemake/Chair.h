#pragma once
#include "Component.h"

class Chair final : public Component
{
public:
	Chair();

	void Start() override;
	void Update(float elapsedSec) override;
	void Draw() const override;
};

