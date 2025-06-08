#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class GameObject;
	class Command;
	class ButtonComponent final: public Component
	{
	public:
		ButtonComponent(float width, float height, GameObject* pOwner);
		~ButtonComponent() override = default;
		
		void Render() const override;
		void OnClick() const;

		void Select();
		void Deselect();

		bool IsSelected() const { return m_IsSelected; };

		void SetCommand(std::shared_ptr<Command> pCommand) { m_pCommand = pCommand; }

		void SetBorderAroundText();

	private:
		float m_Width;
		float m_Height;
		int m_LineWidth{ 2 };

		bool m_IsSelected{ false };
		std::shared_ptr<Command> m_pCommand{ nullptr  };
		
	};
}


