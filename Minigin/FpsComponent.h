#pragma once
#include "Component.h"
#include "TextComponent.h"
namespace dae
{
	class FpsComponent : public Component
	{
	public:
		FpsComponent(std::shared_ptr<Font> font);
		void SetPosition(float x, float y);

		void Update() override;
		void Render() const override;
	private:
		std::shared_ptr<TextComponent> m_FpsTexture;
	};
}

