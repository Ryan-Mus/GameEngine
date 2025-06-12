#pragma once
#include <Command.h>
#include <ServiceLocator.h>
#include <GameObject.h>

#include "PacmanGrid.h"

class MuteCommand : public dae::Command
{
public:
	void Execute() override
	{
		if (!m_IsMuted)
		{
			m_lastVolume = dae::ServiceLocator::GetSoundService().GetMasterVolume();
			dae::ServiceLocator::GetSoundService().SetMasterVolume(0);
			m_IsMuted = true;
		}
		else
		{
			m_IsMuted = false;
			dae::ServiceLocator::GetSoundService().SetMasterVolume(m_lastVolume);
		}
	}
private:
	bool m_IsMuted{ false };
	int m_lastVolume{};
};

class SkipLevelCommand : public dae::Command
{
public:
	SkipLevelCommand(dae::GameObject* pGrid) : m_pGrid{ pGrid } {};
	void Execute() override
	{
		if (m_pGrid) m_pGrid->GetComponent<PacmanGrid>()->GoToNextLevel();
	}
private:
	dae::GameObject* m_pGrid{ nullptr };
};