#pragma once
#include <Component.h>
#include "MsPacmanObserver.h"
class MsPacmanDieComponent final : public dae::Component, public MsPacmanObserver
{
public:
	MsPacmanDieComponent(dae::GameObject* pOwner) : dae::Component(pOwner) {};

	void Update() override;
	void OnNotify(MsPacmanEvent event) override;
private:
	float m_elapsedTime{ 0.f };
	float m_timeToDie{ 1.f }; // Time before respawn
	bool m_IsDying{ false };
};

