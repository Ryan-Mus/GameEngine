#pragma once
#include "BaseMovement.h"
class PacmanMovement : public BaseMovement
{
public:
	explicit PacmanMovement(float speed, dae::GameObject* owner)
		:BaseMovement{ speed, owner }
	{
	};
	void Update() override;
};

