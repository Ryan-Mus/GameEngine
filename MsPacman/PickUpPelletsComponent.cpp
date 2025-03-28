#include "PickUpPelletsComponent.h"

void dae::PickUpPelletsComponent::PickUpPellet(bool isBig)
{
	if (isBig)
	{
		m_ScoreEvent->Notify(EventType::Score100);
	}
	else
	{
		m_ScoreEvent->Notify(EventType::Score10);
	}
}
