#include "ChaseState.h"
#include "GhostMovement.h"
#include "FrightenedState.h"
#include "SpriteSheetAnimatorComponent.h"
#include "GhostStateComponent.h"

void ChaseState::OnEnter()
{
	m_pGhost->GetComponent<GhostMovement>()->SetSpeed(150.f);
	m_pMovement = m_pGhost->GetComponent<GhostMovement>();
	m_pAnimator = m_pGhost->GetComponent<dae::SpriteSheetAnimator>();
	m_Type = m_pGhost->GetComponent<GhostStateComponent>()->GetGhostType();
	m_pGrid = m_pMovement->GetGrid();
}

void ChaseState::OnExit()
{

}

std::unique_ptr<GhostState> ChaseState::Update()
{
	glm::ivec2 target{ 0, 0 };

	switch (m_Type)
	{
		case GhostType::Blinky:
		{
			target = m_pGrid->GetMsPacmanPos();
			break;
		}
		case GhostType::Pinky:
		{
			auto pacmanPos = m_pGrid->GetMsPacmanPos();
			auto pacmanDirection = m_pGhost->GetComponent<GhostMovement>()->GetDirection();
			target = { pacmanPos.x + pacmanDirection.x * 4, pacmanPos.y + pacmanDirection.y * 4 };
			break;
		}
		case GhostType::Inky:
		{
			auto pacmanPos = m_pGrid->GetMsPacmanPos();
			auto pacmanDirection = m_pGhost->GetComponent<GhostMovement>()->GetDirection();
			target = { pacmanPos.x + pacmanDirection.x * 2, pacmanPos.y + pacmanDirection.y * 2 };
			break;
		}
		case GhostType::Clyde:
		{
			auto pacmanPos = m_pGrid->GetMsPacmanPos();
			auto pacmanDirection = m_pGhost->GetComponent<GhostMovement>()->GetDirection();
			target = { pacmanPos.x + pacmanDirection.x * 3, pacmanPos.y + pacmanDirection.y * 3 };
			break;
		}
	default:
		break;
	}

	m_pMovement->SetTarget(target);

	auto direction = m_pMovement->GetDirection();
	
	if (direction.x > 0)
		m_pAnimator->PlayAnimation("Right");
	else if (direction.x < 0)
		m_pAnimator->PlayAnimation("Left");
	else if (direction.y > 0)
		m_pAnimator->PlayAnimation("Down");
	else if (direction.y < 0)
		m_pAnimator->PlayAnimation("Up");
    
    return nullptr;
}

std::unique_ptr<GhostState> ChaseState::OnNotify(MsPacmanEvent event)
{
	if (event == MsPacmanEvent::EATEN_BIG_PELLET)
	{
		return std::make_unique<FrightenedState>(m_pGhost);
	}
	return nullptr;
}
