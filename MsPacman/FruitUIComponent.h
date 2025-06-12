#pragma once
#include "Component.h"
#include "MsPacmanObserver.h"
#include <vector>
class FruitUIComponent: public dae::Component, public MsPacmanObserver
{
public:
	FruitUIComponent(dae::GameObject* pOwner)
	: Component(pOwner) {}

	void OnNotify(MsPacmanEvent e) override;

	void RegisterFruit(dae::GameObject*);

private:
	std::vector<dae::GameObject*> m_pFruits;
};

