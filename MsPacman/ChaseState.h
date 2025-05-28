#pragma once
#include "GhostState.h"
#include "MsPacmanObserver.h"

enum class GhostType;
class PacmanGrid;
class GhostMovement;
namespace dae
{
	class SpriteSheetAnimator;
}
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
	GhostStateType GetStateType() const override { return GhostStateType::CHASE; }
private:
	GhostMovement* m_pMovement;
	dae::SpriteSheetAnimator* m_pAnimator;
	PacmanGrid* m_pGrid;
	GhostType m_Type;
};
