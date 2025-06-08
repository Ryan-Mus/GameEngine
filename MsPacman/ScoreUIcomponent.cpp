#pragma once
#include "ScoreUIcomponent.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "SceneManager.h"
#include "GameLoader.h"

void dae::ScoreUIcomponent::AddScore(int score)
{
	m_Score += score;
	GetOwner()->GetComponent<TextComponent>()->SetText("Score: " + std::to_string(m_Score));
}

void dae::ScoreUIcomponent::ResetScore()
{
	m_Score = 0;
	GetOwner()->GetComponent<TextComponent>()->SetText("Score: " + std::to_string(m_Score));
}

void dae::ScoreUIcomponent::SaveScore()
{
	std::filesystem::path filePath = "../data/lastscore.txt";
	std::ofstream file(filePath);
	if (file.is_open())
	{
		file << "LST ";
		file << m_Score;
		file.close();
	}
	else
	{
		std::cerr << "Unable to open file for writing: " << filePath << std::endl;
	}
}

void dae::ScoreUIcomponent::GoToHighScoreScreen()
{
	// Save the score before transitioning
	SaveScore();
	
	// Request scene change to be processed at the end of the frame
	dae::SceneManager::GetInstance().QueueSceneChange([](){ 
		auto& sceneManager = dae::SceneManager::GetInstance();
		sceneManager.ClearScenes();

		auto& GameLoader = GameLoader::GetInstance();
		GameLoader.loadGameJSON("../data/HighscoreMenu.json");

		sceneManager.SetActiveScene("HighscoreMenu");
	});
}
