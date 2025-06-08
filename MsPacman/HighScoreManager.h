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

	void AddHighscoreDisplay(dae::GameObject* pDisplayObject, int index);
	void SetLastHighScoreDisplay(dae::GameObject* pDisplayObject);

	void LoadHighScores();
	void SaveHighScore();
private:
	HighScore m_LastHighScore{};
	dae::GameObject* m_pLastHighScoreDisplayObject{ nullptr };

	std::array<HighScore, 10> m_HighScores{};
	std::array<dae::GameObject*, 10> m_pHighScoreDisplayObjects{};
};

