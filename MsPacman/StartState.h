#pragma once
#include "GhostState.h"

class StartState : public GhostState
{
public:
	StartState(dae::GameObject* ghost) : GhostState(ghost) { OnEnter(); }
	std::unique_ptr<GhostState> Update() override;
private:
	float m_Timer{ 0.f };
	float m_Duration{ 5.f };
};

