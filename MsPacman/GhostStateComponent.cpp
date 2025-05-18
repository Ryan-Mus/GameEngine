#include "GhostStateComponent.h"
#include <iostream>

void GhostStateComponent::Update()
{
  auto state = m_State->Update();
  if (state)
  {
	  m_State->OnExit();
      m_State = std::move(state);
	  m_State->OnEnter();
  }
}

void GhostStateComponent::OnNotify(MsPacmanEvent event)
{
	auto state  = m_State->OnNotify(event);

	if (state)
	{
		m_State->OnExit();
		m_State = std::move(state);
		m_State->OnEnter();
	}

	if (event == MsPacmanEvent::START_LEVEL)
	{
		m_State->OnEnter();
	}
}
