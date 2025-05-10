#pragma once
#include "GhostState.h"
#include "MsPacmanObserver.h"

class GhostStateComponent final : public dae::Component, public MsPacmanObserver
{
public:
    GhostStateComponent(std::unique_ptr<GhostState> state, dae::GameObject* pOwner)
        : dae::Component{ pOwner }, m_State{ std::move(state) } {};
    virtual void Update() override; // Ensure this matches the implementation

    virtual void OnNotify(MsPacmanEvent event) override;
private:
    std::unique_ptr<GhostState> m_State;
};
