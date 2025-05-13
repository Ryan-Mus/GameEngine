#pragma once
#include "GhostState.h"
#include "MsPacmanObserver.h"

class ChaseState : public GhostState
{
public:
	ChaseState(dae::GameObject* pGhost)
		: GhostState(pGhost) {}
	// Override the Update method to implement the chase behavior
	void OnEnter() override;
	void OnExit() override;
	std::unique_ptr<GhostState> Update() override;

	std::unique_ptr<GhostState> OnNotify(MsPacmanEvent event) override;
private:
};
