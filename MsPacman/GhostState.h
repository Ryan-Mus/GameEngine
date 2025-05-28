#pragma once
#include "GameObject.h"
#include "MsPacmanObserver.h"

enum class GhostStateType
{
	CHASE,
	FRIGHTENED,
	EATEN,
	START
};

class GhostState
{
public:
	GhostState(dae::GameObject* pGhost);
	virtual ~GhostState();
	virtual std::unique_ptr<GhostState> Update() { return nullptr; }
	virtual void OnEnter() {};
	virtual void OnExit() {};
	virtual std::unique_ptr<GhostState> OnNotify(MsPacmanEvent) { return nullptr; };
	virtual GhostStateType GetStateType() const = 0;
protected:
	dae::GameObject* m_pGhost{ nullptr };
};