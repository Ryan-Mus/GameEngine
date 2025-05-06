#pragma once
#include "GhostState.h"

class GhostStateComponent final : public dae::Component
{
public:
    GhostStateComponent(std::unique_ptr<GhostState> state, dae::GameObject* pOwner)
        : dae::Component{ pOwner }, m_State{ std::move(state) } {};
    virtual void Update() override; // Ensure this matches the implementation
private:
    std::unique_ptr<GhostState> m_State;
};
