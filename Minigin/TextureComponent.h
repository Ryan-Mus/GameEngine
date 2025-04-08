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
		explicit TextureComponent(const std::string& filename, int x, int y, int width, int height, int scale, GameObject* pOwner);

		void SetSource(int SrcX, int SrcY, int width, int height)
		{
			x = SrcX;
			y = SrcY;
			w = width;
			h = height;
		};

		void SetScale(int scale)
		{
			s = scale;
		}

		void SetOffset(int offSetX, int offSetY)
		{
			offsetX = offSetX;
			offsetY = offSetY;
		}
		virtual void Render() const override;

		Texture2D* GetTexture() const
		{
			return m_Texture.get();
		}
	private:
		std::shared_ptr<Texture2D> m_Texture;
		int x, y, w, h, s;
		int offsetX, offsetY;
	};
}

