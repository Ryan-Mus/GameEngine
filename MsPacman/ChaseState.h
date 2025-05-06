#pragma once
#include "GhostState.h"

class ChaseState : public GhostState
{
public:
	explicit ChaseState(dae::GameObject* pGhost) : GhostState{ pGhost } {};
	// Override the Update method to implement the chase behavior
	std::unique_ptr<GhostState> Update() override;
};
