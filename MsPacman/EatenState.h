#pragma once
#include "GameObject.h"
#include "GhostState.h"

class EatenState : public GhostState
{
public:
	EatenState(dae::GameObject* pGhost)
		: GhostState(pGhost) {}
	// Override the Update method to implement the evade behavior
	std::unique_ptr<GhostState> Update() override;
	void OnEnter() override;
	void OnExit() override;

	std::unique_ptr<GhostState> OnNotify(MsPacmanEvent) override;
private:
};

