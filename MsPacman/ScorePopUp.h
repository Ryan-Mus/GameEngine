#pragma once
#include <Component.h>
#include <vector>
#include "MsPacmanObserver.h"
#include "PacmanGrid.h"


class ScorePopUp final : public dae::Component, public MsPacmanObserver
{
public:
	ScorePopUp(dae::GameObject* pOwner);

	void Update() override;

	void OnNotify(MsPacmanEvent e) override;

	void SetGrid(PacmanGrid* grid) { m_pGrid = grid; }
private:
	PacmanGrid* m_pGrid{ nullptr };
	float m_ElapsedTime{ 0.0f };
	int m_Streak{ 0 };
};

