#pragma once
#include "GhostState.h"

class StartState : public GhostState
{
public:
	StartState(dae::GameObject* ghost);
	void OnEnter() override;
	std::unique_ptr<GhostState> Update() override;

	GhostStateType GetStateType() const override { return GhostStateType::START; }
private:
	float m_Timer{ 0.f };
	float m_Duration{ 5.f };
};

