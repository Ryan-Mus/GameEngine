#include "ChaseState.h"
#include "GhostMovement.h"

std::unique_ptr<GhostState> ChaseState::Update()
{
	auto grid = m_pGhost->GetComponent<GhostMovement>()->GetGrid();

	if (!grid)
		return nullptr;

	auto target = grid->GetMsPacmanPos();
	m_pGhost->GetComponent<GhostMovement>()->SetTarget(target);
    
    return nullptr;
}
