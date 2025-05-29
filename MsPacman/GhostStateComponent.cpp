#include "GhostStateComponent.h"
#include "StartState.h"
#include "PacmanGrid.h"
#include "GhostMovement.h"
#include "SpriteSheetAnimatorComponent.h"
#include <iostream>

GhostStateComponent::GhostStateComponent(std::unique_ptr<GhostState> state, GhostType type, glm::ivec2 startPos, dae::GameObject* pOwner)
	: dae::Component{ pOwner }, m_State{ std::move(state) }, m_GhostType{ type }, m_StartPos{ startPos }
{
	auto grid = GetOwner()->GetComponent<GhostMovement>()->GetGrid();
	GetOwner()->SetLocalPostion({ grid->GridToLocalPosition(m_StartPos.x, m_StartPos.y), 0 });
}

void GhostStateComponent::Update()
{
  auto state = m_State->Update();
  if (state)
  {
	  m_State->OnExit();
      m_State = std::move(state);
	  m_State->OnEnter();
  }
}

void GhostStateComponent::OnNotify(MsPacmanEvent event)
{
	auto state  = m_State->OnNotify(event);

	if (state)
	{
		m_State->OnExit();
		m_State = std::move(state);
		m_State->OnEnter();
	}

	if (event == MsPacmanEvent::DIE || event == MsPacmanEvent::START_LEVEL)
	{
		m_State->OnExit();
		m_State = std::make_unique<StartState>(GetOwner());
		m_State->OnEnter();

		GhostMovement* movement = GetOwner()->GetComponent<GhostMovement>();
		auto grid = movement->GetGrid();

		movement->SetTarget(m_StartPos);
		GetOwner()->SetLocalPostion({ grid->GridToLocalPosition(m_StartPos.x,m_StartPos.y),0 });

		auto animation = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
		animation->PlayAnimation("Up");
	}
}
