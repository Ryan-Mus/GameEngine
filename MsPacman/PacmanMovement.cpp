#include "PacmanMovement.h"
#include <TimeSingleton.h>
#include <GameObject.h>
#include <ServiceLocator.h>

void PacmanMovement::Update()
{
	glm::vec3 translation = m_Direction * m_Speed * dae::Time::GetInstance().GetDeltaTime();
	GetOwner()->Translate(translation.x, translation.y, translation.z);
}
