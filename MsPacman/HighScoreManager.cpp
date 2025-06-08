#include "HighScoreManager.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

#include "TextComponent.h"
#include "GameObject.h"
#include "ButtonComponent.h"

HighScoreManager::HighScoreManager(dae::GameObject* pOwner)
    : Component(pOwner)
{
}

void HighScoreManager::AddHighscoreDisplay(dae::GameObject* pDisplayObject, int index)
{
    if (index < 0 || index >= m_pHighScoreDisplayObjects.size())
    {
        return; // Invalid index
    }
    m_pHighScoreDisplayObjects[index] = pDisplayObject;
	pDisplayObject->GetComponent<dae::TextComponent>()->SetText("AAA 0");
}

void HighScoreManager::AddNameDisplay(dae::GameObject* pDisplayObject, int index)
{
    if (index < 0 || index >= m_pNameDisplayObjects.size())
    {
        return; // Invalid index
    }
    m_pNameDisplayObjects[index] = pDisplayObject;
	pDisplayObject->GetComponent<dae::TextComponent>()->SetText("A");
}

void HighScoreManager::SetLastHighScoreDisplay(dae::GameObject* pDisplayObject)
{
    m_pLastHighScoreDisplayObject = pDisplayObject;
}

void HighScoreManager::ChangeLetter(bool increase)
{
    for (int i{}; i < 3; ++i)
    {
        auto button = m_pNameDisplayObjects[i]->GetComponent<dae::ButtonComponent>();
        if (button->IsSelected())
        {
            auto textComp = m_pNameDisplayObjects[i]->GetComponent<dae::TextComponent>();
            char letter = textComp->GetText()[0];
            if (increase)
            {
                if (letter >= 'Z' || letter < 'A')
                    letter = 'A';
                else if (letter >= 'A' && letter < 'Z')
                    ++letter;
            }
            else
            {
                if (letter <= 'A' || letter > 'Z')
                    letter = 'Z';
                else if (letter > 'A' && letter <= 'Z')
                    --letter;
            }
            m_LastHighScore.name[i] = letter;
            textComp->SetText(std::string(1, letter));
            break; // Only change the selected one
        }
    }
}

void HighScoreManager::LoadHighScores()
{
    // Initialize with default values
    for (auto& score : m_HighScores)
    {
        score.score = 0;
        score.name = { 'A', 'A', 'A' };
    }

    // Load high scores from file
    std::ifstream highScoreFile("../data/highscore.txt");
    if (highScoreFile.is_open())
    {
        std::string line;
        size_t idx = 0;

        while (std::getline(highScoreFile, line) && idx < m_HighScores.size())
        {
            if (line.length() >= 5) // At minimum "XXX 0"
            {
                // Extract name (first 3 chars)
                for (int i = 0; i < 3; ++i)
                {
                    m_HighScores[idx].name[i] = line[i];
                }

                // Extract score (everything after the space)
                size_t spacePos = line.find(' ');
                if (spacePos != std::string::npos && spacePos + 1 < line.length())
                {
                    try
                    {
                        m_HighScores[idx].score = std::stoi(line.substr(spacePos + 1));
                    }
                    catch (const std::exception&)
                    {
                        m_HighScores[idx].score = 0;
                    }
                }

                ++idx;
            }
        }

        highScoreFile.close();
    }

    // Load the last high score from lastscore.txt
    std::ifstream lastScoreFile("../data/lastscore.txt");
    if (lastScoreFile.is_open())
    {
        std::string line;
        if (std::getline(lastScoreFile, line) && line.length() >= 5)
        {
            // Extract name (first 3 chars)
            for (int i = 0; i < 3; ++i)
            {
                m_LastHighScore.name[i] = line[i];
            }

            // Extract score (everything after the space)
            size_t spacePos = line.find(' ');
            if (spacePos != std::string::npos && spacePos + 1 < line.length())
            {
                try
                {
                    m_LastHighScore.score = std::stoi(line.substr(spacePos + 1));
                }
                catch (const std::exception&)
                {
                    m_LastHighScore.score = 0;
                }
            }
        }

        lastScoreFile.close();
    }

    for (int i{}; i < 10; ++i)
    {
        m_pHighScoreDisplayObjects[i]->GetComponent<dae::TextComponent>()->SetText(
            std::string(m_HighScores[i].name.begin(), m_HighScores[i].name.end()) + " " +
            std::to_string(m_HighScores[i].score));
	}

    if (m_pLastHighScoreDisplayObject)
    {
        m_pLastHighScoreDisplayObject->GetComponent<dae::TextComponent>()->SetText(
            std::to_string(m_LastHighScore.score));
    }

	// Update the name display objects
    for(int i = 0; i < 3; ++i)
    {
        if (i < m_pNameDisplayObjects.size() && m_pNameDisplayObjects[i])
        {
            m_pNameDisplayObjects[i]->GetComponent<dae::TextComponent>()->SetText(
                std::string(1, m_LastHighScore.name[i]));
        }
	}
	std::cout << "High scores loaded successfully." << std::endl;
}

void HighScoreManager::SaveHighScore()
{
    if (m_HighScores[0].name[0] == 0)
    {
        std::cout << "No high scores to save." << std::endl;
        return; // No high scores to save
    }

    for (size_t i = 0; i < m_HighScores.size(); ++i)
    {
        if (m_LastHighScore.score > m_HighScores[i].score)
        {
            // Shift down all lower scores
            for (size_t j = m_HighScores.size() - 1; j > i; --j)
            {
                m_HighScores[j] = m_HighScores[j - 1];
            }

            // Insert the last high score
            m_HighScores[i] = m_LastHighScore;
            break;
        }
    }
    // Write the high scores to highscore.txt
    std::ofstream highScoreFile("../data/highscore.txt");
    if (highScoreFile.is_open())
    {
        for (const auto& highScore : m_HighScores)
        {
            highScoreFile << highScore.name[0] << highScore.name[1] << highScore.name[2]
                << " " << highScore.score << std::endl;
        }
        highScoreFile.close();
    }

	std::ofstream lastScoreFile("../data/lastscore.txt");
    if (lastScoreFile.is_open())
    {
        lastScoreFile << "A" << "A" << "A"
            << " " << 0 << std::endl;
        lastScoreFile.close();
	}

	std::cout << "High scores saved successfully." << std::endl;
	LoadHighScores();
}