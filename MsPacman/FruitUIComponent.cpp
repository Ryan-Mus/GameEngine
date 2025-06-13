#include "FruitUIComponent.h"
#include <SpriteSheetAnimatorComponent.h>
#include <GameObject.h>
#include <ServiceLocator.h>

void FruitUIComponent::OnNotify(MsPacmanEvent e)
{
	if (m_pFruits.size() != 3) return;
	switch (e)
	{
		case MsPacmanEvent::EATEN_CHERRY:
		{
			auto anim = m_pFruits[0]->GetComponent<dae::SpriteSheetAnimator>();
			anim->PlayAnimation("true");
			dae::ServiceLocator::GetSoundService().PlaySound("fruit");
			break;
		}
		case MsPacmanEvent::EATEN_STRAWBERRY:
		{
			auto anim = m_pFruits[1]->GetComponent<dae::SpriteSheetAnimator>();
			anim->PlayAnimation("true");
			dae::ServiceLocator::GetSoundService().PlaySound("fruit");
			break;
		}
		case MsPacmanEvent::EATEN_ORANGE:
		{
			auto anim = m_pFruits[2]->GetComponent<dae::SpriteSheetAnimator>();
			anim->PlayAnimation("true");
			dae::ServiceLocator::GetSoundService().PlaySound("fruit");
			break;
		}
		default:
			break;
	}
}

void FruitUIComponent::RegisterFruit(dae::GameObject* pFruit)
{
	m_pFruits.push_back(pFruit);
	auto anim = pFruit->GetComponent<dae::SpriteSheetAnimator>();
	anim->PlayAnimation("false");
}