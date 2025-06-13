#pragma once
#include "GameLoader.h"
#include "Command.h"
#include "SceneManager.h"
#include <SDL.h>
#include <iostream>

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
		std::cout << "Solo game loaded." << std::endl;
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
		std::cout << "Co-op game loaded." << std::endl;
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
		std::cout << "Versus game loaded." << std::endl;
	}
};

class OpenHighscoreMenuCommand : public dae::Command
{
public:
	void Execute() override
	{
		std::cout << "Opening Highscore Menu..." << std::endl;
		auto& sceneManager = dae::SceneManager::GetInstance();
		sceneManager.ClearScenes();

		auto& GameLoader = GameLoader::GetInstance();
		GameLoader.loadGameJSON("../data/HighscoreMenu.json");

		sceneManager.SetActiveScene("HighscoreMenu");
		std::cout << "Highscore Menu loaded." << std::endl;
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


