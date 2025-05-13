#pragma once
#include "FrightenedState.h"
#include "ChaseState.h"
#include "GhostMovement.h"
#include "TimeSingleton.h"
#include "SpriteSheetAnimatorComponent.h"
#include "EatenState.h"

std::unique_ptr<GhostState> FrightenedState::Update()
{
    m_Timer += dae::Time::GetInstance().GetDeltaTime();

    if (m_Timer >= m_Duration)
    {
        return std::make_unique<ChaseState>(m_pGhost);
    }
    if (m_Timer > 5.f && !m_EndAnimation)
    {
        m_pGhost->GetComponent<dae::SpriteSheetAnimator>()->PlayAnimation("FrightenedEnd");
        m_EndAnimation = true;
    }

    auto grid = m_pGhost->GetComponent<GhostMovement>()->GetGrid();

    if (!grid)
        return nullptr;

    // Get Ms. Pac-Man's position
    auto pacmanPos = grid->GetMsPacmanPos();

	if (m_pGhost->GetComponent<GhostMovement>()->GetGridPosition() == pacmanPos)
	{
		return std::make_unique<EatenState>(m_pGhost);
	}

    // Calculate the opposite position
    glm::ivec2 gridDimensions = grid->GetDimensions(); // Assuming GetDimensions() returns {width, height}
    glm::ivec2 oppositePos = { gridDimensions.x - pacmanPos.x - 1, gridDimensions.y - pacmanPos.y - 1 };

    // Set the target to the opposite position
    m_pGhost->GetComponent<GhostMovement>()->SetTarget(oppositePos);

    return nullptr;
}


void FrightenedState::OnEnter()
{
	m_pGhost->GetComponent<GhostMovement>()->SetSpeed(100.f);
	m_pGhost->GetComponent<dae::SpriteSheetAnimator>()->PlayAnimation("Frightened");
}

void FrightenedState::OnExit()
{
	m_pGhost->GetComponent<GhostMovement>()->SetSpeed(150.f);
}

std::unique_ptr<GhostState> FrightenedState::OnNotify(MsPacmanEvent)
{
	return nullptr;
}