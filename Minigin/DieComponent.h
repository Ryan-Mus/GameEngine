#pragma once
#include "Component.h"
#include "Subject.h"
#include <memory>

namespace dae
{
	class DieComponent : public Component
	{
	public:
		DieComponent(std::unique_ptr<Subject> subject, GameObject* pOwner)
			:Component(pOwner), m_DieEvent{ std::move(subject) } {};
		void Die();

		void AddObserver(Observer* observer)
		{
			m_DieEvent->AddObserver(observer);
		}

		void RemoveObserver(Observer* observer)
		{
			m_DieEvent->RemoveObserver(observer);
		}
	private:
		std::unique_ptr<Subject> m_DieEvent;
	};
}

