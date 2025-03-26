#pragma once
#include "Component.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "Transform.h"

#include <memory>
namespace dae
{
	class TextureComponent final: public Component
	{
	public:
		explicit TextureComponent(const std::string& filename,GameObject* pOwner);
		explicit TextureComponent(const std::string& filename,int x,int y, int width,int height, GameObject* pOwner);
		virtual void Render() const override;
	private:
		std::shared_ptr<Texture2D> m_Texture;
		int x, y, w, h;
	};
}

