#pragma once
#include "Command.h"
#include "GameObject.h"
#include "ButtonManagerComponent.h"

namespace dae
{
	class SelectNextButtonCommand final : public Command
	{
	public:
		SelectNextButtonCommand(GameObject* pButtonManager) 
				: m_pButtonManager(pButtonManager->GetComponent<ButtonManagerComponent>()) {}

		virtual void Execute() override
		{
			if (m_pButtonManager)
			{
				m_pButtonManager->SelectNextButton();
			}
		}
	private:
		ButtonManagerComponent* m_pButtonManager;
	};

	class SelectPreviousButtonCommand final : public Command
	{
	public:
		SelectPreviousButtonCommand(GameObject* pButtonManager) :
			m_pButtonManager(pButtonManager->GetComponent<ButtonManagerComponent>()) {}

		virtual void Execute() override
		{
			if (m_pButtonManager)
			{
				m_pButtonManager->SelectPreviousButton();
			}
		}
	private:
		ButtonManagerComponent* m_pButtonManager;
	};

	class PressSelectedButtonCommand final : public Command
	{
	public:
		PressSelectedButtonCommand(GameObject* pButtonManager) :
			m_pButtonManager(pButtonManager->GetComponent<ButtonManagerComponent>()) {
		}

		virtual void Execute() override
		{
			if (m_pButtonManager)
			{
				m_pButtonManager->PressSelectedButton();
			}
		}
	private:
		ButtonManagerComponent* m_pButtonManager;
	};
}