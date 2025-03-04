#pragma once
#include "Command.h"
#include "GameObject.h"
#include "Time.h"
namespace dae
{
	class MoveUpCommand final : public Command
	{
	public:
		MoveUpCommand(float speed, GameObject* pOwner) :m_Speed{speed},m_GameObject { pOwner } {};
		void Execute() override 
		{
			m_GameObject->Translate(0, -m_Speed*Time::GetInstance().GetDeltaTime(), 0);
		}
	private:
		float m_Speed;
		GameObject* m_GameObject;
	};
	class MoveRightCommand final : public Command
	{
	public:
		MoveRightCommand(float speed, GameObject* pOwner) :m_Speed{ speed }, m_GameObject{ pOwner } {};
		void Execute() override
		{
			m_GameObject->Translate(m_Speed * Time::GetInstance().GetDeltaTime(), 0, 0);
		}
	private:
		float m_Speed;
		GameObject* m_GameObject;
	};
	class MoveDownCommand final : public Command
	{
	public:
		MoveDownCommand(float speed, GameObject* pOwner) :m_Speed{ speed }, m_GameObject{ pOwner } {};
		void Execute() override
		{
			m_GameObject->Translate(0, m_Speed * Time::GetInstance().GetDeltaTime(), 0);
		}
	private:
		float m_Speed;
		GameObject* m_GameObject;
	};
	class MoveLeftCommand final : public Command
	{
	public:
		MoveLeftCommand(float speed, GameObject* pOwner) :m_Speed{ speed }, m_GameObject{ pOwner } {};
		void Execute() override
		{
			m_GameObject->Translate(-m_Speed * Time::GetInstance().GetDeltaTime(), 0, 0);
		}
	private:
		float m_Speed;
		GameObject* m_GameObject;
	};
}