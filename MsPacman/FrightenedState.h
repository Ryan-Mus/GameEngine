#pragma once
#include "GhostState.h"
#include "FrightenedState.h"
#include "CustomPacmanDefines.h"

class FrightenedState : public GhostState
{
public:
	FrightenedState(dae::GameObject* pGhost)
		: GhostState(pGhost)
	{
		OnEnter();
	}
	// Override the Update method to implement the evade behavior
	std::unique_ptr<GhostState> Update() override;
	void OnEnter() override;
	void OnExit() override;

	std::unique_ptr<GhostState> OnNotify(MsPacmanEvent event) override;
	GhostStateType GetStateType() const override { return GhostStateType::FRIGHTENED; }
private:
	float m_Timer{ 0.f };
	float m_Duration{ PACMAN_FRIGHTENED_DURATION };
	bool m_EndAnimation{ false };
};

