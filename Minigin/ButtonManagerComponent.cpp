#include "ButtonManagerComponent.h"
#include "ButtonComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

#include <iostream>
#include <algorithm>

dae::ButtonManagerComponent::ButtonManagerComponent(GameObject* pOwner)
	:Component(pOwner)
{
}

void dae::ButtonManagerComponent::Update()
{
	if(m_NeedsToUpdate)
	{
		// Hacky solution to resize buttons based on text size (text size is set on the first frame)
		for(auto button : m_pButtons)
		{
			auto textComponent = button->GetComponent<TextComponent>();
			if (textComponent)
			{
				auto size = textComponent->GetSize();
				if(size == glm::ivec2(0, 0))
				{
					return;
				}
			}
			ButtonComponent* buttonComponent = button->GetComponent<ButtonComponent>();
		
			buttonComponent->SetBorderAroundText();	
		}
		m_NeedsToUpdate = false;
	}
}

void dae::ButtonManagerComponent::AddButton(const GameObject* pButton)
{
	if (pButton != nullptr)
	{
		ButtonComponent* button = pButton->GetComponent<ButtonComponent>();
		if (button != nullptr)
		{
			m_pButtons.push_back(pButton);
			if (m_pButtons.size() == 1)
			{
				button->Select();
				m_SelectedButtonIndex = 0; // Set the first button as selected
			}
		}
		else
		{
			std::cout << "ButtonManagerComponent::AddButton: The provided GameObject does not have a ButtonComponent." << std::endl;
		}
	}
	else
	{
		std::cout << "ButtonManagerComponent::AddButton: The provided GameObject is null." << std::endl;
	}
}

void dae::ButtonManagerComponent::RemoveButton(const GameObject* pButton)
{
	if (pButton != nullptr)
	{
		auto it = std::find(m_pButtons.begin(), m_pButtons.end(), pButton);
		if (it != m_pButtons.end())
		{
			m_pButtons.erase(it);
			m_SelectedButtonIndex = 0; // Reset selected index
		}
	}
}

void dae::ButtonManagerComponent::SelectNextButton()
{
	if(m_pButtons.empty())
		return;
	
	ButtonComponent* oldButton = m_pButtons[m_SelectedButtonIndex]->GetComponent<ButtonComponent>();
	oldButton->Deselect();

	m_SelectedButtonIndex = (m_SelectedButtonIndex + 1) % m_pButtons.size();

	ButtonComponent* newButton = m_pButtons[m_SelectedButtonIndex]->GetComponent<ButtonComponent>();
	newButton->Select();
}

void dae::ButtonManagerComponent::SelectPreviousButton()
{
	if(m_pButtons.empty())
		return;
	
	ButtonComponent* oldButton = m_pButtons[m_SelectedButtonIndex]->GetComponent<ButtonComponent>();
	oldButton->Deselect();

	m_SelectedButtonIndex = (m_SelectedButtonIndex - 1 + m_pButtons.size()) % m_pButtons.size();

	ButtonComponent* newButton = m_pButtons[m_SelectedButtonIndex]->GetComponent<ButtonComponent>();
	newButton->Select();
}

void dae::ButtonManagerComponent::PressSelectedButton()
{
	if (m_pButtons.empty())
		return;
	ButtonComponent* button = m_pButtons[m_SelectedButtonIndex]->GetComponent<ButtonComponent>();

	button->OnClick();
}
