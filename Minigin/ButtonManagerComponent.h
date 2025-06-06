#pragma once
#include "Component.h"
#include <vector>

namespace dae
{
	class GameObject;
	class ButtonManagerComponent : public Component
	{
	public:
		ButtonManagerComponent(GameObject* pOwner);
		~ButtonManagerComponent() override = default;

		void Update() override;

		void AddButton(const GameObject* pButton);
		void RemoveButton(const GameObject* pButton);

		void SelectNextButton();
		void SelectPreviousButton();

		void PressSelectedButton();
	private:
		std::vector<const GameObject*> m_pButtons;
		size_t m_SelectedButtonIndex{ 0 };

		bool m_NeedsToUpdate{ true };
	};
}

