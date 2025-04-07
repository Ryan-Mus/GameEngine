#pragma once
#include <String>

#include "LivesUIComponent.h"
#include "GameObject.h"
#include "TextComponent.h"


void dae::LivesUIComponent::LoseLife()
{
	m_Lives--;
	GetOwner()->GetComponent<TextComponent>()->SetText("Lives: " + std::to_string(m_Lives));
}

void dae::LivesUIComponent::ResetLives()
{
	m_Lives = 3;
	GetOwner()->GetComponent<TextComponent>()->SetText("Lives: " + std::to_string(m_Lives));
}
