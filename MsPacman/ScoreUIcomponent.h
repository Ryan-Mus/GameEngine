#pragma once
#include "Component.h"
#include "TextComponent.h"
#include "MsPacmanObserver.h"
#include "ServiceLocator.h"

namespace dae
{
	class ScoreUIcomponent : public Component, public MsPacmanObserver
	{
	public:
		ScoreUIcomponent(GameObject* pOwner):Component(pOwner) {};
		void AddScore(int score);
		void ResetScore();

		void OnNotify(MsPacmanEvent event) override
		{
			switch (event)
			{
			case MsPacmanEvent::EATEN_SMALL_PELLET:
				AddScore(10);
				dae::ServiceLocator::GetSoundService().PlaySound("eatPellet", 0);
				break;
			case MsPacmanEvent::EATEN_BIG_PELLET:
				AddScore(50);
				dae::ServiceLocator::GetSoundService().PlaySound("eatBigPellet", 0);
				break;
			}
		}
	private:
		int m_Score = 0;
	};
}

