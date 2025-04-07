#pragma once
#include <iostream>

#include "DieComponent.h"


void dae::DieComponent::Die()
{
	m_DieEvent->Notify(EventType::PlayerDied);
	std::cout << "Player died\n";
}
