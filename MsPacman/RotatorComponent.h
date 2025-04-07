#pragma once
#include "Component.h"

namespace dae
{
	class RotatorComponent final : public Component
	{
	public:
		RotatorComponent(float speed, GameObject* pOwner)
			: Component{ pOwner }, m_Speed{speed} {}

		void Update() override;
	private:
		float m_Speed;
	};
}

