#include "EatenState.h"
#include "GhostMovement.h"
#include "ChaseState.h"
#include "SpriteSheetAnimatorComponent.h"

std::unique_ptr<GhostState> EatenState::Update()
{
	// Check if the ghost is at the target position
	auto movement = m_pGhost->GetComponent<GhostMovement>();
	if (movement->GetGridPosition() == movement->GetTarget())
	{
		return std::make_unique<ChaseState>(m_pGhost);
	}

	auto anim = m_pGhost->GetComponent<dae::SpriteSheetAnimator>();
	auto dir = movement->GetDirection();

	// 
	if (dir.x > 0)
		anim->PlayAnimation("EyesRight");
	else if (dir.x < 0)
		anim->PlayAnimation("EyesLeft");
	else if (dir.y > 0)
		anim->PlayAnimation("EyesDown");
	else if (dir.y < 0)
		anim->PlayAnimation("EyesUp");

	return nullptr;
}

void EatenState::OnEnter()
{
	m_pGhost->GetComponent<GhostMovement>()->SetTarget({ 12, 14 });
	m_pGhost->GetComponent<GhostMovement>()->SetSpeed(300.f);
}

void EatenState::OnExit()
{
	m_pGhost->GetComponent<GhostMovement>()->SetSpeed(150.f);
}

std::unique_ptr<GhostState> EatenState::OnNotify(MsPacmanEvent)
{
	return nullptr;
}
