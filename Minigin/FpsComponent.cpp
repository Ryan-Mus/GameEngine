#pragma once
#include <iostream>

#include "FpsComponent.h"
#include "TimeSingleton.h"
#include "TextComponent.h"
#include "GameObject.h"

dae::FpsComponent::FpsComponent(GameObject* pOwner)
	:Component{pOwner}
{
}

void dae::FpsComponent::Update()
{
	float fps = 1 / Time::GetInstance().GetDeltaTime();
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << fps;//1 decimal
	GetOwner()->GetComponent<TextComponent>()->SetText(ss.str());
}
