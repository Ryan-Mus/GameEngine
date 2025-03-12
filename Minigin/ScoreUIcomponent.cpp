#include "ScoreUIcomponent.h"
#include "CSteamAchievements.h"

void dae::ScoreUIcomponent::AddScore(int score)
{
	m_Score += score;
	GetOwner()->GetComponent<TextComponent>()->SetText("Score: " + std::to_string(m_Score));

	if (m_Score >= 500)
	{
		g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
	}
}

void dae::ScoreUIcomponent::ResetScore()
{
	m_Score = 0;
	GetOwner()->GetComponent<TextComponent>()->SetText("Score: " + std::to_string(m_Score));
}
