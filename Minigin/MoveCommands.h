#pragma once
#include <glm.hpp>

#include "Command.h"
#include "GameObject.h"
#include "TimeSingleton.h"
namespace dae
{
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(float speed, glm::vec2 direction, GameObject* pOwner) :m_Speed{ speed }, m_Direction{direction}, m_GameObject{ pOwner } {};
		void Execute() override 
		{
			m_GameObject->Translate(m_Direction.x * m_Speed * Time::GetInstance().GetDeltaTime(),
			m_Direction.y * m_Speed * Time::GetInstance().GetDeltaTime(), 0);
		}
	private:
		float m_Speed;
		GameObject* m_GameObject;
		glm::vec2 m_Direction;
	};
}