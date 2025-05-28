#pragma once
#include "Command.h"
#include "GameObject.h"
#include "PickUpPelletsComponent.h"

namespace dae
{
	class PickUpPelletsCommand final : public Command
	{
	public:
		PickUpPelletsCommand(bool isBig, GameObject* pOwner) :m_IsBig{isBig} ,m_GameObject { pOwner } {};
		void Execute() override
		{
			m_GameObject->GetComponent<PickUpPelletsComponent>()->PickUpPellet(m_IsBig);
		}
	private:
		GameObject* m_GameObject;
		bool m_IsBig;
	};
}