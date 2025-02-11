#include "TextureComponent.h"
#include "ResourceManager.h"

dae::TextureComponent::TextureComponent(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::Render() const
{
	if (m_Texture == nullptr) return;
	const auto pos{ m_Transform.GetPosition() };
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}
