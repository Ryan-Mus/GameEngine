#pragma once
#include "Command.h"
#include "GameObject.h"
#include "HighscoreManager.h"
#include <iostream>
#include "SceneManager.h"
#include "GameLoader.h"
class LoadHighscoreCommand : public dae::Command
{
public:
	LoadHighscoreCommand(dae::GameObject* pHighscoreManager) 
		:m_pHighscoreManager(pHighscoreManager->GetComponent<HighScoreManager>())
	{
	}

	void Execute() override
	{
		if (m_pHighscoreManager)
		{
			m_pHighscoreManager->LoadHighScores();
		}
		else
		{
			std::cout << "HighScoreManager component not found!" << std::endl;
		}
	}
private:
	 HighScoreManager* m_pHighscoreManager;
};

class SaveHighscoreCommand : public dae::Command
{
	public:
	SaveHighscoreCommand(dae::GameObject* pHighscoreManager)
		: m_pHighscoreManager(pHighscoreManager->GetComponent<HighScoreManager>())
	{
	}
	void Execute() override
	{
		if (m_pHighscoreManager)
		{
			m_pHighscoreManager->SaveHighScore();
		}
		else
		{
			std::cout << "HighScoreManager component not found!" << std::endl;
		}
	}
private:
	HighScoreManager* m_pHighscoreManager;
};

class OpenMainMenuCommand : public dae::Command
{
public:
	void Execute() override
	{
		std::cout << "Opening Main menu..." << std::endl;
		auto& sceneManager = dae::SceneManager::GetInstance();
		sceneManager.ClearScenes();

		auto& GameLoader = GameLoader::GetInstance();
		GameLoader.loadGameJSON("../data/MainMenu.json");
		sceneManager.SetActiveScene("MainMenu");
	}
};
