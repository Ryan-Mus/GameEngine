#pragma once
#include <Command.h>
#include "GhostStateComponent.h"

class PlayerGhostMoveCommand : public dae::Command
{
public:
	PlayerGhostMoveCommand(GhostStateComponent* ghostState, glm::ivec2 direction)
		: m_GhostState(ghostState), m_Direction(direction) {
	}
	void Execute() override
	{
		if (m_GhostState)
		{
			m_GhostState->SetPlayerDirection(m_Direction);
		}
	}
private:
	GhostStateComponent* m_GhostState;
	glm::ivec2 m_Direction;
};
