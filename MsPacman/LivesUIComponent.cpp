#pragma once
#include <String>

#include "LivesUIComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Renderer.h"
#include "CustomPacmanDefines.h"

#include <ServiceLocator.h>
#include <TimeSingleton.h>


void LivesUIComponent::Render() const
{
	for (int i{}; i < m_Lives; ++i)
	{
		float offsetX = i * 16.f * PACMAN_RENDERSCALE;
		dae::Renderer::GetInstance().RenderTexture(*m_pTexture, GetOwner()->GetWorldPosition().x + offsetX, GetOwner()->GetWorldPosition().y,472, 0, 16, 16, PACMAN_RENDERSCALE);
	}
}

void LivesUIComponent::Update()
{
	if (m_IsImmune)
	{
		m_ImmuneTimer += dae::Time::GetInstance().GetDeltaTime();
		if (m_ImmuneTimer >= m_ImmuneDuration)
		{
			m_IsImmune = false;
			m_ImmuneTimer = 0.f;
		}
	}
}

void LivesUIComponent::LoseLife()
{
	m_Lives--;
	dae::ServiceLocator::GetSoundService().PlaySound("die");
}

void LivesUIComponent::ResetLives()
{
	m_Lives = 3;
}

void LivesUIComponent::OnNotify(MsPacmanEvent e)
{
	if (m_IsImmune) return;
	if (e == MsPacmanEvent::DIE)
	{
		LoseLife();
		m_IsImmune = true;
	}
}
