#pragma once
#include "GameObject.h"

class GhostState
{
public:
	GhostState(dae::GameObject* pGhost);
	virtual std::unique_ptr<GhostState> Update() { return nullptr; }
protected:
	dae::GameObject* m_pGhost{ nullptr };
};