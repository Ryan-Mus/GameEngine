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

class StartCoopLevelCommand final: public dae::Command
{
	public:
	virtual void Execute() override
	{
		dae::SceneManager::GetInstance().SetActiveScene("Coop");
	}
};

class StartVersusLevelCommand final: public dae::Command
{
	public:
	virtual void Execute() override
	{
		dae::SceneManager::GetInstance().SetActiveScene("Versus");
	}
};


class QuitGameCommand final: public dae::Command
{
	public:
	virtual void Execute() override
	{
		SDL_Event quitEvent;
		quitEvent.type = SDL_QUIT;
		SDL_PushEvent(&quitEvent);
	}
};


