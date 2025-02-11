#include "FpsComponent.h"
#include "Time.h"
#include <iostream>
dae::FpsComponent::FpsComponent(std::shared_ptr<Font> font) :m_FpsTexture(std::make_shared<TextComponent>("0", font))
{
}
void dae::FpsComponent::SetPosition(float x, float y)
{
	m_FpsTexture->SetPosition(x, y);
}
void dae::FpsComponent::Update()
{
	float fps = 1 / Time::GetInstance().GetDeltaTime();
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << fps;//1 decimal
	m_FpsTexture->SetText(ss.str());
	m_FpsTexture->Update();
}

void dae::FpsComponent::Render() const
{
	m_FpsTexture->Render();
}
