#pragma once
#include <String>

#include "LivesUIComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Renderer.h"
#include "CustomPacmanDefines.h"


void LivesUIComponent::Render() const
{
	for (int i{}; i < m_Lives; ++i)
	{
		float offsetX = i * 16.f * PACMAN_RENDERSCALE;
		dae::Renderer::GetInstance().RenderTexture(*m_pTexture, GetOwner()->GetWorldPosition().x + offsetX, GetOwner()->GetWorldPosition().y,472, 0, 16, 16, PACMAN_RENDERSCALE);
	}
}

void LivesUIComponent::LoseLife()
{
	m_Lives--;
}

void LivesUIComponent::ResetLives()
{
	m_Lives = 4;
}
