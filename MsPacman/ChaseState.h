#pragma once
#include "GhostState.h"
#include "MsPacmanObserver.h"

class ChaseState : public GhostState
{
public:
	ChaseState(dae::GameObject* pGhost)
		: GhostState(pGhost)
	{
		OnEnter();
	}
	// Override the Update method to implement the chase behavior
	void OnEnter() override;
	void OnExit() override;
	std::unique_ptr<GhostState> Update() override;

	void OnNotify(MsPacmanEvent event) override;
private:
	bool m_IsChasing{ true };
};
