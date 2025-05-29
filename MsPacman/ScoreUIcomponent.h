#pragma once
#include "Component.h"
#include "TextComponent.h"
#include "MsPacmanObserver.h"
#include "ServiceLocator.h"
#include "TimeSingleton.h"
#include "CustomPacmanDefines.h"

namespace dae
{
	class ScoreUIcomponent : public Component, public MsPacmanObserver
	{
	public:
		ScoreUIcomponent(GameObject* pOwner):Component(pOwner) {};
		void AddScore(int score);
		void ResetScore();

		void Update() override
		{
			if(m_Streak > 0)
			{
				m_ElapsedTime += Time::GetInstance().GetDeltaTime();
			}
			if (m_ElapsedTime > PACMAN_FRIGHTENED_DURATION)
			{
				m_ElapsedTime = 0.0f;
				m_Streak = 0;
			}
		}

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
				m_Streak = 0; // Reset streak on big pellet
				m_ElapsedTime = 0; // Reset elapsed time on big pellet
				break;
			case MsPacmanEvent::EATEN_GHOST:
				++m_Streak;
				switch (m_Streak)
				{
				case 1:
					AddScore(200);
					break;
				case 2:
					AddScore(400);
					break;
				case 3:
					AddScore(800);
					break;
				case 4:
					AddScore(1600);
					break;
				default:
					AddScore(0); // No score for streaks beyond 4
					break;
				}
				break;
			case MsPacmanEvent::EATEN_CHERRY:
				AddScore(100);
				break;
			case MsPacmanEvent::EATEN_STRAWBERRY:
				AddScore(200);
				break;
			case MsPacmanEvent::EATEN_ORANGE:
				AddScore(500);
				break;

			}
		}
	private:
		int m_Score = 0;
		int m_Streak = 0;
		float m_ElapsedTime = 0;
	};
}

