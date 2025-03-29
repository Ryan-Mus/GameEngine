#pragma once
#include <memory>

#include "Component.h"
#include "MsPacmanSubject.h"
namespace dae
{
	class PickUpPelletsComponent : public Component
	{
	public:
		PickUpPelletsComponent(std::unique_ptr<MsPacmanSubject>scoreEvent, GameObject* pOwner) 
			: Component{ pOwner }, m_ScoreEvent{std::move(scoreEvent)} {};

		void PickUpPellet(bool isBig);

		void AddObserver(MsPacmanObserver* observer)
		{
			m_ScoreEvent->AddObserver(observer);
		}

		void RemoveObserver(MsPacmanObserver* observer)
		{
			m_ScoreEvent->RemoveObserver(observer);
		}
	private:
		std::unique_ptr<MsPacmanSubject> m_ScoreEvent;
	};
}

