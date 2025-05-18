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
    GhostStateComponent(std::unique_ptr<GhostState> state,GhostType type, dae::GameObject* pOwner)
		: dae::Component{ pOwner }, m_State{ std::move(state) }, m_GhostType{ type } {};
    virtual void Update() override; // Ensure this matches the implementation

    virtual void OnNotify(MsPacmanEvent event) override;

	GhostType GetGhostType() const
	{
		return m_GhostType;
	}
private:
    std::unique_ptr<GhostState> m_State;
	GhostType m_GhostType{ GhostType::Blinky };
};
