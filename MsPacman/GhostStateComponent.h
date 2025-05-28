#pragma once
#include "GhostState.h"

#include "MsPacmanObserver.h"

enum class GhostType
{
	Blinky,
	Pinky,
	Inky,
	Clyde
};

class GhostStateComponent final : public dae::Component, public MsPacmanObserver
{
public:
	GhostStateComponent(std::unique_ptr<GhostState> state, GhostType type, glm::ivec2 startPos, dae::GameObject* pOwner);

    virtual void Update() override; // Ensure this matches the implementation

    virtual void OnNotify(MsPacmanEvent event) override;

	GhostType GetGhostType() const
	{
		return m_GhostType;
	}

    GhostState* GetState() const { return m_State.get(); }
private:
    std::unique_ptr<GhostState> m_State;
	GhostType m_GhostType{ GhostType::Blinky };
	glm::ivec2 m_StartPos{ 0, 0 };
};
