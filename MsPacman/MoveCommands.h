#pragma once
#include <glm.hpp>

#include "Command.h"
#include "GameObject.h"
#include "TimeSingleton.h"
#include "PacmanMovement.h"
#include "SpriteSheetAnimatorComponent.h"
namespace dae
{
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(glm::vec2 direction, GameObject* pOwner) :m_Direction{direction}, m_GameObject{ pOwner } {};
		void Execute() override 
		{
			m_GameObject->GetComponent<PacmanMovement>()->SetDirection({ m_Direction.x,m_Direction.y,0.f});
			auto animator = m_GameObject->GetComponent<SpriteSheetAnimator>();

			if (m_Direction.x > 0)
				animator->PlayAnimation("Right");
			else if (m_Direction.x < 0)
				animator->PlayAnimation("Left");
			else if (m_Direction.y > 0)
				animator->PlayAnimation("Down");
			else if (m_Direction.y < 0)
				animator->PlayAnimation("Up");
		}
	private:
		GameObject* m_GameObject;
		glm::vec2 m_Direction;
	};
}