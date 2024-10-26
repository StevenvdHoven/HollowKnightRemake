#pragma once
#include "GameObject.h"



class Collider;
class Player final : public GameObject
{
public:
	Player();

	void StopInput();
	void ResumeInput();

};

