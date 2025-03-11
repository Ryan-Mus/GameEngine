#pragma once
#include "Component.h"
#include "TextComponent.h"
#include "Observer.h"

namespace dae 
{
	class LivesUIComponent : public Component, public Observer
	{
	public:
		LivesUIComponent(GameObject* pOwner):Component(pOwner) {};

		void LoseLife();
		void ResetLives();

		void OnNotify(EventType e) override { if(e == EventType::PlayerDied) LoseLife(); }
	private:
		int m_Lives = 3;
	};
}

