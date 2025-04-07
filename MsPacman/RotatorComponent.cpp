#pragma once
#include "RotatorComponent.h"
#include "GameObject.h"
#include "TimeSingleton.h"

void dae::RotatorComponent::Update()
{
	float deltaTime = Time::GetInstance().GetDeltaTime();

	GetOwner()->Rotate(0,0,m_Speed*deltaTime);
}
