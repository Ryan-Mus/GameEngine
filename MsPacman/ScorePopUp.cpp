#include "ScorePopUp.h"
#include "TimeSingleton.h"
#include "SpriteSheetAnimatorComponent.h"
#include "CustomPacmanDefines.h"
#include <iostream>

ScorePopUp::ScorePopUp(dae::GameObject* pOwner)
	: dae::Component(pOwner)
{
}

void ScorePopUp::Update()
{
	if (m_Streak > 0)
	{
		m_ElapsedTime += dae::Time::GetInstance().GetDeltaTime();
	}
	if (m_ElapsedTime > PACMAN_FRIGHTENED_DURATION)
	{
		m_ElapsedTime = 0.0f;
		m_Streak = 0;
	}
}

void ScorePopUp::OnNotify(MsPacmanEvent e)
{
	glm::ivec2 pos;
	if (m_pGrid->IsScoreForMsPacman())
	{
		pos = m_pGrid->GetMsPacmanPos();
	}
	else
	{
		pos = m_pGrid->GetPacmanPos();
	}

	if (e == MsPacmanEvent::EATEN_GHOST)
	{
		++m_Streak;
		auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();

		GetOwner()->SetLocalPostion({
			(pos.x * m_pGrid->GetCellSize()),
			(pos.y * m_pGrid->GetCellSize()),
			0});

		switch (m_Streak)
		{
		case 1:
			animator->PlayAnimation("200");
			std::cout << "Ghost eaten score: 200" << std::endl;
			break;
		case 2:
			animator->PlayAnimation("400");
			std::cout << "Ghost eaten score: 400" << std::endl;
			break;
		case 3:
			animator->PlayAnimation("800");
			std::cout << "Ghost eaten score: 800" << std::endl;
			break;
		case 4:
			animator->PlayAnimation("1600");
			std::cout << "Ghost eaten score: 1600" << std::endl;
			break;
		}
	}

	else if(e == MsPacmanEvent::EATEN_CHERRY)
	{
		auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
		animator->PlayAnimation("cherry");
		GetOwner()->SetLocalPostion({
			(pos.x * m_pGrid->GetCellSize()),
			(pos.y * m_pGrid->GetCellSize()),
			0 });
	}

	else if (e == MsPacmanEvent::EATEN_STRAWBERRY)
	{
		auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
		animator->PlayAnimation("strawberry");
		GetOwner()->SetLocalPostion({
			(pos.x * m_pGrid->GetCellSize()),
			(pos.y * m_pGrid->GetCellSize()),
			0 });
	}

	else if (e == MsPacmanEvent::EATEN_ORANGE)
	{
		auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
		animator->PlayAnimation("orange");
		GetOwner()->SetLocalPostion({
			(pos.x * m_pGrid->GetCellSize()),
			(pos.y * m_pGrid->GetCellSize()),
			0 });
	}

	else if (e == MsPacmanEvent::EATEN_BIG_PELLET)
	{
		m_ElapsedTime = 0.0f;
		m_Streak = 0;
	}
}
