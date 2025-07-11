#pragma once
#include <string>
#include <memory>

#include "GameObject.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public Component
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		const std::string& GetText() const { return m_text; }

		explicit TextComponent(const std::string& text, std::shared_ptr<Font> font, GameObject* owner)
			:Component{ owner }, m_NeedsUpdate(true), m_text(text), m_font(std::move(font)), m_textTexture(nullptr) {};

		glm::ivec2 GetSize() const;
		
	private:
		bool m_NeedsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_textTexture;
	};
}
