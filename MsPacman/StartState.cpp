#include "StartState.h"
#include "TimeSingleton.h"
#include "ChaseState.h"

std::unique_ptr<GhostState> StartState::Update()
{
	m_Timer += dae::Time::GetInstance().GetDeltaTime();

	if (m_Timer >= m_Duration)
	{
		return std::make_unique<ChaseState>(m_pGhost);
	}

	return nullptr;
}
