#pragma once
#include "Component.h"
#include <array>
#
class HighScoreManager : public dae::Component
{
	struct HighScore
	{
		int score;
		std::array<char,3> name;
	};

public:
	HighScoreManager(dae::GameObject* pOwner);
private:
	void LoadHighScores();
	void SaveHighScore();

	HighScore LastHighScore{};

	std::array<HighScore, 10> m_HighScores{};
	std::array<dae::GameObject*, 10> m_HighScoreDisplayObjects{};
};

