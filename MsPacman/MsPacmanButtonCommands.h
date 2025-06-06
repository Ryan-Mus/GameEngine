#pragma once
#include "Command.h"
#include "SceneManager.h"

class StartSoloLevelCommand final: public dae::Command
{
public:
	virtual void Execute() override
	{
		dae::SceneManager::GetInstance().SetActiveScene("Solo");
	}
};

class QuitGameCommand final: public dae::Command
{
	public:
	virtual void Execute() override
	{
		exit(0);
	}
};
