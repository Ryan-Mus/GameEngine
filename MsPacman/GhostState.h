#pragma once
#include "GameObject.h"
#include "MsPacmanObserver.h"

class GhostState
{
public:
	GhostState(dae::GameObject* pGhost);
	virtual ~GhostState();
	virtual std::unique_ptr<GhostState> Update() { return nullptr; }
	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual std::unique_ptr<GhostState> OnNotify(MsPacmanEvent) { return nullptr; };
protected:
	dae::GameObject* m_pGhost{ nullptr };
};