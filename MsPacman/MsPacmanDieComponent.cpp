#include "MsPacmanDieComponent.h"
#include "SpriteSheetAnimatorComponent.h"
#include "TimeSingleton.h"
#include "GameObject.h"
#include "PacmanGrid.h"
#include "PacmanMovement.h"

void MsPacmanDieComponent::Update()
{
	if (m_IsDying)
	{
		m_elapsedTime += dae::Time::GetInstance().GetDeltaTime();	

		if (m_elapsedTime >= m_timeToDie)
		{
			m_IsDying = false;
			m_elapsedTime = 0.f;

			auto grid = GetOwner()->GetComponent<PacmanMovement>()->GetGrid();
			GetOwner()->SetLocalPostion({ grid->GridToLocalPosition(14, 23),0 });
			GetOwner()->GetComponent<PacmanMovement>()->ToggleMovement();
		}
	}
}

void MsPacmanDieComponent::OnNotify(MsPacmanEvent event)
{
	if (event == MsPacmanEvent::DIE && !m_IsDying)
	{
		m_IsDying = true;
		m_elapsedTime = 0.f;
		GetOwner()->GetComponent<dae::SpriteSheetAnimator>()->PlayAnimation("Death");
		GetOwner()->GetComponent<PacmanMovement>()->ToggleMovement();
	}
}
