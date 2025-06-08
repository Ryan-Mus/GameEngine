#pragma once
#include "GameLoader.h"

class StartSoloLevelCommand final: public dae::Command
{
public:
	virtual void Execute() override
	{
		std::cout << "Opening Solo game..." << std::endl;
		auto& sceneManager = dae::SceneManager::GetInstance();
		sceneManager.ClearScenes();

		auto& GameLoader = GameLoader::GetInstance();
		GameLoader.loadGameJSON("../data/Solo.json");
		sceneManager.SetActiveScene("Solo");
	}
};

class StartCoopLevelCommand final: public dae::Command
{
	public:
	virtual void Execute() override
	{
		std::cout << "Opening Co-op game..." << std::endl;
		auto& sceneManager = dae::SceneManager::GetInstance();
		sceneManager.ClearScenes();

		auto& GameLoader = GameLoader::GetInstance();
		GameLoader.loadGameJSON("../data/Coop.json");
		sceneManager.SetActiveScene("Coop");
	}
};

class StartVersusLevelCommand final: public dae::Command
{
	public:
	virtual void Execute() override
	{
		std::cout << "Opening Versus game..." << std::endl;
		auto& sceneManager = dae::SceneManager::GetInstance();
		sceneManager.ClearScenes();

		auto& GameLoader = GameLoader::GetInstance();
		GameLoader.loadGameJSON("../data/Versus.json");
		sceneManager.SetActiveScene("Versus");
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


