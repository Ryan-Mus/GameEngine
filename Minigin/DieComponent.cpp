#include "DieComponent.h"
#include <iostream>

void dae::DieComponent::Die()
{
	m_DieEvent->Notify(EventType::PlayerDied);
	std::cout << "Player died\n";
}
