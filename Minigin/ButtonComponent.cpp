#include "ButtonComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Command.h"
#include <SDL.h>

dae::ButtonComponent::ButtonComponent(float width, float height, GameObject* pOwner)
	: Component{pOwner}
	, m_Width{ width }
	, m_Height{height}
{
}

void dae::ButtonComponent::Render() const
{
	auto pos = GetOwner()->GetWorldPosition();
	Renderer::GetInstance().RenderRectangle(pos.x,pos.y, m_Width, m_Height, SDL_Color{ 255, 255, 255, 255 }, m_LineWidth);
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
