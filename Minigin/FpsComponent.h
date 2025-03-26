#pragma once
#include "Component.h"

namespace dae
{
	class GameObject;
	class FpsComponent final : public Component
	{
	public:
		FpsComponent(GameObject* pOwner);

		void Update() override;
	private:
		
	};
}

