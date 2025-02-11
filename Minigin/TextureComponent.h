#pragma once
#include "Component.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "Transform.h"
#include <memory>
namespace dae
{
	class TextureComponent : public Component
	{
	public:
		TextureComponent(const std::string& filename);
		virtual void Render() const override;

		void SetPosition(float x, float y) { m_Transform.SetPosition(x, y, 0.0f); }
	private:
		std::shared_ptr<Texture2D> m_Texture;
		Transform m_Transform{};
	};
}

