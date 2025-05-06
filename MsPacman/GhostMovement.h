#pragma once
#include "BaseMovement.h"
class GhostMovement : public BaseMovement
{
public:
	explicit GhostMovement(float speed, dae::GameObject* owner)
		:BaseMovement{ speed, owner }
	{
	};
	void Update() override;
	glm::ivec2 CalculateNextStep(const std::pair<int, int>& start
		, const glm::ivec2& goal);
	void SetTarget(const glm::ivec2& target) { m_Target = target; }
private:
	glm::ivec2 m_Target{ 0, 0 };
};

