#pragma once
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"

dae::TextureComponent::TextureComponent(const std::string& filename, GameObject* pOwner)
	: Component{ pOwner }, x{}, y{}, w{}, h{}
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

dae::TextureComponent::TextureComponent(const std::string& filename, int x, int y, int width, int height, GameObject* pOwner)
	: Component{ pOwner } ,x {x}, y{y}, w{width}, h{height}
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

dae::TextureComponent::TextureComponent(const std::string& filename, int x, int y, int width, int height,int scale, GameObject* pOwner)
	: Component{ pOwner }, x{ x }, y{ y }, w{ width }, h{ height }, s{scale}
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::Render() const
{
	if (m_Texture == nullptr) return;
	const glm::vec3 pos{ GetOwner()->GetWorldPosition()};
	if (w == 0 || h == 0)Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	else Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, x, y, w, h,s);
}
