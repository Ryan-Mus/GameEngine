#include "FruitBehavior.h"
#include "SpriteSheetAnimatorComponent.h"
#include "GameObject.h"
#include "GhostMovement.h"

#include <iostream>

FruitBehavior::FruitBehavior(dae::GameObject* owner)
: dae::Component(owner)
{
	GetOwner()->SetLocalPostion({ -100,0,0 });
	m_Path.reserve(10);
	m_Path.push_back({ 18, 17 });
	m_Path.push_back({ 9, 17 });
	m_Path.push_back({ 9, 11 });
	m_Path.push_back({ 18, 11 });
	m_Path.push_back({ 18, 17 });
	m_Path.push_back({ 0, 17 });

}

void FruitBehavior::Update()
{
	if (m_IsEaten || !m_IsSpawned)
	{
		return;
	}
		

	auto movement = GetOwner()->GetComponent<GhostMovement>();
	if (m_Path[m_CurrentPathIndex] == movement->GetGridPosition())
	{
		++m_CurrentPathIndex;
		if (m_CurrentPathIndex >= m_Path.size())
		{
			m_IsSpawned = false;
			m_CurrentPathIndex = 0;
			auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
			animator->PlayAnimation("Idle");
			return;
		}
	}
	movement->SetTarget(m_Path[m_CurrentPathIndex]);
}

void FruitBehavior::OnNotify(MsPacmanEvent e)
{
	switch (e)
	{
		case MsPacmanEvent::SPAWN_FRUIT:
		{
			std::cout << "Fruit spawned" << std::endl;
			m_IsSpawned = true;
			m_IsEaten = false;
			auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
			auto movement = GetOwner()->GetComponent<GhostMovement>();
			

			switch (m_FruitType)
			{
			case FruitType::Cherry:
				animator->PlayAnimation("Cherry");
				GetOwner()->SetLocalPostion({ movement->GetGrid()->GridToLocalPosition(27,8) ,0 });
				break;
			case FruitType::Straweberry:
				animator->PlayAnimation("Strawberry");
				GetOwner()->SetLocalPostion({ movement->GetGrid()->GridToLocalPosition(27,1) ,0 });
				break;
			case FruitType::Orange:
				animator->PlayAnimation("Orange");
				GetOwner()->SetLocalPostion({ movement->GetGrid()->GridToLocalPosition(27,9) ,0 });
				break;
			default:
				break;
			}

			if (m_IsEaten)
			{
				m_IsEaten = false;
				//movement->ToggleMovement();
			}

			break;
		}
		case MsPacmanEvent::LEVEL_COMPLETE:
		{
			m_FruitType++;

			switch (m_FruitType)
			{
			case FruitType::Cherry:
				break;
			case FruitType::Straweberry:
				m_Path.clear();
				m_Path.push_back({ 18, 17 });
				m_Path.push_back({ 9, 17 });
				m_Path.push_back({ 9, 11 });
				m_Path.push_back({ 18, 11 });
				m_Path.push_back({ 18, 17 });
				m_Path.push_back({ 0,23 });
			
				break;
			case FruitType::Orange:
				m_Path.clear();
				m_Path.push_back({ 18, 17 });
				m_Path.push_back({ 9, 17 });
				m_Path.push_back({ 9, 11 });
				m_Path.push_back({ 18, 11 });
				m_Path.push_back({ 18, 17 });
				m_Path.push_back({ 0, 9 });

				break;
			default:
				break;
			}
		}
		case MsPacmanEvent::START_LEVEL:
		{
			m_IsSpawned = false;
			m_CurrentPathIndex = 0;
			auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
			animator->PlayAnimation("Idle");

			break;
		}
		case MsPacmanEvent::EATEN_CHERRY:
		{
			std::cout << "Cherry eaten" << std::endl;
			m_IsEaten = true;
			auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
			animator->PlayAnimation("Idle");
			break;
		}
		case MsPacmanEvent::EATEN_STRAWBERRY:
		{
			std::cout << "Strawberry eaten" << std::endl;
			m_IsEaten = true;
			auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
			animator->PlayAnimation("Idle");
			break;
		}
		case MsPacmanEvent::EATEN_ORANGE:
		{
			std::cout << "Orange eaten" << std::endl;
			m_IsEaten = true;
			auto animator = GetOwner()->GetComponent<dae::SpriteSheetAnimator>();
			animator->PlayAnimation("Idle");
			break;
		}
		default:
			break;
	}
}
