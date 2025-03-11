#include "ScoreUIcomponent.h"

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
