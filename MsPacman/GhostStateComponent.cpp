#include "GhostStateComponent.h"
#include <iostream>

void GhostStateComponent::Update()
{
    if (m_State)
    {
        auto state = m_State->Update();
        if (state)
        {
            m_State = std::move(state);
        }
    }
    else
    {
		std::cout << "GhostState is not initialized." << std::endl;
        // Uncomment this if you want to throw an exception when m_State is not initialized
        // throw std::runtime_error("GhostState is not initialized.");
    }
}
