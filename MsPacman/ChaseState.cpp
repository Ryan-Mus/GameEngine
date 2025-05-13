#include "ChaseState.h"
#include "GhostMovement.h"
#include "FrightenedState.h"
#include "SpriteSheetAnimatorComponent.h"

void ChaseState::OnEnter()
{
	m_pGhost->GetComponent<GhostMovement>()->SetSpeed(150.f);
}

void ChaseState::OnExit()
{

}

std::unique_ptr<GhostState> ChaseState::Update()
{
	auto movement = m_pGhost->GetComponent<GhostMovement>();
	auto grid = movement->GetGrid();

	if (!grid)
		return nullptr;

	auto target = grid->GetMsPacmanPos();
	movement->SetTarget(target);

	auto direction = movement->GetDirection();
	auto animation = m_pGhost->GetComponent<dae::SpriteSheetAnimator>();

	if (direction.x > 0)
		animation->PlayAnimation("Right");
	else if (direction.x < 0)
		animation->PlayAnimation("Left");
	else if (direction.y > 0)
		animation->PlayAnimation("Down");
	else if (direction.y < 0)
		animation->PlayAnimation("Up");
    
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
