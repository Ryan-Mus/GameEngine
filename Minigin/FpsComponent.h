#pragma once
#include "Component.h"
#include "TextComponent.h"
namespace dae
{
	class FpsComponent final : public Component
	{
	public:
		FpsComponent(GameObject* pOwner);

		void Update() override;
	private:
		
	};
}

