#include "StartState.h"
#include "TimeSingleton.h"
#include "ChaseState.h"
#include "GhostMovement.h"
#include "GhostStateComponent.h"

StartState::StartState(dae::GameObject* ghost)
	: GhostState(ghost)
{
	
}

void StartState::OnEnter()
{
	auto type = m_pGhost->GetComponent<GhostStateComponent>()->GetGhostType();
	switch (type)
	{
	case GhostType::Blinky:
		m_Duration = 5.f;
		break;
	case GhostType::Pinky:
		m_Duration = 7.f;
		break;
	case GhostType::Inky:
		m_Duration = 10.f;
		break;
	case GhostType::Clyde:
		m_Duration = 15.f;
		break;
	}
}

std::unique_ptr<GhostState> StartState::Update()
{
	m_Timer += dae::Time::GetInstance().GetDeltaTime();

	if (m_Timer >= m_Duration)
	{
		return std::make_unique<ChaseState>(m_pGhost);
	}

	return nullptr;
}
