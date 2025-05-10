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
    }
}

void GhostStateComponent::OnNotify(MsPacmanEvent event)
{
	if (m_State)
	{
		m_State->OnNotify(event);
	}
	else
	{
		std::cout << "GhostState is not initialized." << std::endl;
	}
}
