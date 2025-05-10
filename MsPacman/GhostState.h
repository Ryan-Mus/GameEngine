#pragma once
#include "GameObject.h"
#include "MsPacmanObserver.h"

class GhostState: public MsPacmanObserver
{
public:
	GhostState(dae::GameObject* pGhost);
	virtual ~GhostState();
	virtual std::unique_ptr<GhostState> Update() { return nullptr; }
	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual void OnNotify(MsPacmanEvent) override {};
protected:
	dae::GameObject* m_pGhost{ nullptr };
};