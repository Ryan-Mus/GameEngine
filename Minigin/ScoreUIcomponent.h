#pragma once
#include "Component.h"
#include "TextComponent.h"
#include "Observer.h"
namespace dae
{
	class ScoreUIcomponent : public Component, public Observer
	{
	public:
		ScoreUIcomponent(GameObject* pOwner):Component(pOwner) {};
		void AddScore(int score);
		void ResetScore();

		void OnNotify(EventType event) override
		{
			switch (event)
			{
			case EventType::Score10:
				AddScore(10);
				break;
			case EventType::Score100:
				AddScore(100);
				break;
			}
		}
	private:
		int m_Score = 0;
	};
}

