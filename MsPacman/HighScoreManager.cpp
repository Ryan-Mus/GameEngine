#include "HighScoreManager.h"
#include <fstream>
#include <string>
#include <algorithm>

HighScoreManager::HighScoreManager(dae::GameObject* pOwner)
    : Component(pOwner)
{
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
    std::ifstream highScoreFile("highscore.txt");
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
    std::ifstream lastScoreFile("lastscore.txt");
    if (lastScoreFile.is_open())
    {
        std::string line;
        if (std::getline(lastScoreFile, line) && line.length() >= 5)
        {
            // Extract name (first 3 chars)
            for (int i = 0; i < 3; ++i)
            {
                LastHighScore.name[i] = line[i];
            }

            // Extract score (everything after the space)
            size_t spacePos = line.find(' ');
            if (spacePos != std::string::npos && spacePos + 1 < line.length())
            {
                try
                {
                    LastHighScore.score = std::stoi(line.substr(spacePos + 1));
                }
                catch (const std::exception&)
                {
                    LastHighScore.score = 0;
                }
            }
        }

        lastScoreFile.close();
    }
}

void HighScoreManager::SaveHighScore()
{
    // Check if LastHighScore should be included in the high scores
    bool inserted = false;
    for (size_t i = 0; i < m_HighScores.size(); ++i)
    {
        if (LastHighScore.score > m_HighScores[i].score)
        {
            // Shift down all lower scores
            for (size_t j = m_HighScores.size() - 1; j > i; --j)
            {
                m_HighScores[j] = m_HighScores[j - 1];
            }

            // Insert the last high score
            m_HighScores[i] = LastHighScore;
            inserted = true;
            break;
        }
    }

    // Write the high scores to highscore.txt
    std::ofstream highScoreFile("highscore.txt");
    if (highScoreFile.is_open())
    {
        for (const auto& highScore : m_HighScores)
        {
            highScoreFile << highScore.name[0] << highScore.name[1] << highScore.name[2]
                << " " << highScore.score << std::endl;
        }
        highScoreFile.close();
    }

	std::ofstream lastScoreFile("lastscore.txt");
    if (lastScoreFile.is_open())
    {
        lastScoreFile << "A" << "A" << "A"
            << " " << 0 << std::endl;
        lastScoreFile.close();
	}
}