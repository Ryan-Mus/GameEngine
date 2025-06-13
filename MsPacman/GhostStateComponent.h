#pragma once
#include "GhostState.h"

#include "MsPacmanObserver.h"

enum class GhostType
{
	Blinky,
	Pinky,
	Inky,
	Clyde,
	PlayerGhost,
};

class GhostStateComponent final : public dae::Component, public MsPacmanObserver
{
public:
	GhostStateComponent(std::unique_ptr<GhostState> state, GhostType type, glm::ivec2 startPos, dae::GameObject* pOwner);

    virtual void Update() override;

    virtual void OnNotify(MsPacmanEvent event) override;

	GhostType GetGhostType() const
	{
		return m_GhostType;
	}

	glm::ivec2 GetPlayerDirection() const { return m_PlayerDirection; }
	void SetPlayerDirection(glm::ivec2 direction) { m_PlayerDirection = direction; }

    GhostState* GetState() const { return m_State.get(); }
private:
    std::unique_ptr<GhostState> m_State;
	GhostType m_GhostType{ GhostType::Blinky };
	glm::ivec2 m_StartPos{ 0, 0 };
	glm::ivec2 m_PlayerDirection{ 0, 0 };
};
