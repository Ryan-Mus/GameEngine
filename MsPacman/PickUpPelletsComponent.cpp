#include "PickUpPelletsComponent.h"

void dae::PickUpPelletsComponent::PickUpPellet(bool isBig)
{
	if (isBig)
	{
		m_ScoreEvent->Notify(MsPacmanEvent::EATEN_BIG_PELLET);
	}
	else
	{
		m_ScoreEvent->Notify(MsPacmanEvent::EATEN_SMALL_PELLET);
	}
}
