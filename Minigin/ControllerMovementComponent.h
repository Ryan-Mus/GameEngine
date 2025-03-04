#pragma once
#include "Component.h"

namespace dae
{
	class ControllerMovementComponent final : public Component
	{
	public:
		ControllerMovementComponent(float speed, int playerIndex, GameObject* pOwner);
	private:
	};

	
}

