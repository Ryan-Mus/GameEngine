#pragma once
#include "Component.h"
#include <memory>
#include "Subject.h"
namespace dae
{
	class PickUpPelletsComponent : public Component
	{
	public:
		PickUpPelletsComponent(std::unique_ptr<Subject>scoreEvent, GameObject* pOwner) 
			: Component{ pOwner }, m_ScoreEvent{std::move(scoreEvent)} {};

		void PickUpPellet(bool isBig);

		void AddObserver(Observer* observer)
		{
			m_ScoreEvent->AddObserver(observer);
		}

		void RemoveObserver(Observer* observer)
		{
			m_ScoreEvent->RemoveObserver(observer);
		}
	private:
		std::unique_ptr<Subject> m_ScoreEvent;
	};
}

