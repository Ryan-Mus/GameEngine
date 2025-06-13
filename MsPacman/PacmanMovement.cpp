#include "PacmanMovement.h"
#include <TimeSingleton.h>

void PacmanMovement::Update()
{
    if(m_ElapsedTime < m_StartRoundDuration)
    {
        m_ElapsedTime += dae::Time::GetInstance().GetDeltaTime();
        return; // Skip movement during the start round duration
	}
    BaseMovement::Update();
}

