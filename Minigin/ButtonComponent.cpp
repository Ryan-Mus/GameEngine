#include "ButtonComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Command.h"
#include "TextComponent.h"
#include <SDL.h>
#include <iostream>

dae::ButtonComponent::ButtonComponent(float width, float height, GameObject* pOwner)
	: Component{pOwner}
	, m_Width{ width + 10.f }
	, m_Height{height + 10.f}
{
}

void dae::ButtonComponent::Render() const
{
	auto pos = GetOwner()->GetWorldPosition();
	Renderer::GetInstance().RenderRectangle(pos.x-5.f,pos.y-5.f, m_Width, m_Height, SDL_Color{ 255, 255, 255, 255 }, m_LineWidth);
}

void dae::ButtonComponent::OnClick() const
{
	if(m_IsSelected && m_pCommand)
	{
		m_pCommand->Execute();
	}
}

void dae::ButtonComponent::Select()
{
	m_IsSelected = true;
	m_LineWidth = 4;
}

void dae::ButtonComponent::Deselect()
{
	m_IsSelected = false;
	m_LineWidth = 1;
}

void dae::ButtonComponent::SetBorderAroundText()
{
	auto text = GetOwner()->GetComponent<TextComponent>();
	if (text)
	{
		auto size = text->GetSize();
		m_Width = size.x + 10.f; // Add some padding
		m_Height = size.y + 10.f; // Add some padding

		std::cout << "ButtonComponent::SetBorderAroundText: Width: " << m_Width << ", Height: " << m_Height << std::endl;
	}
}
