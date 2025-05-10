#pragma once
#include "GhostState.h"

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

	void OnNotify(MsPacmanEvent event) override;
private:
	float m_Timer{ 0.f };
	float m_Duration{ 7.f };
	bool m_EndAnimation{ false };
};

