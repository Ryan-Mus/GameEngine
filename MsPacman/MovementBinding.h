#include "InputManager.h"
#include "MoveCommands.h"
#include "XInputDefines.h"

namespace dae 
{
	void AddControllerMovement(int playerIndex, GameObject* pOwner)
	{
		auto& input = InputManager::GetInstance();
		input.AddController(playerIndex);

		input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN,
			playerIndex,
			KeyState::Down,
			std::make_shared<MoveCommand>(glm::vec2(0, 1), pOwner));
		input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP,
			playerIndex,
			KeyState::Down,
			std::make_shared<MoveCommand>(glm::vec2(0, -1), pOwner));
		input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT,
			playerIndex,
			KeyState::Down,
			std::make_shared<MoveCommand>(glm::vec2(-1, 0), pOwner));
		input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT,
			playerIndex,
			KeyState::Down,
			std::make_shared<MoveCommand>(glm::vec2(1, 0), pOwner));
	}

	void RemoveControllerMovement(int playerIndex)
	{
		auto& input = InputManager::GetInstance();
		input.RemoveControllerBinding(XINPUT_GAMEPAD_DPAD_DOWN, playerIndex, KeyState::Down);
		input.RemoveControllerBinding(XINPUT_GAMEPAD_DPAD_UP, playerIndex, KeyState::Down);
		input.RemoveControllerBinding(XINPUT_GAMEPAD_DPAD_LEFT, playerIndex, KeyState::Down);
		input.RemoveControllerBinding(XINPUT_GAMEPAD_DPAD_RIGHT, playerIndex, KeyState::Down);
	}

	void AddKeyboardMovement(GameObject* pOwner)
	{
		auto& input = InputManager::GetInstance();

		input.BindKeyboardCommand('W'
			, KeyState::Down,
			std::make_shared<MoveCommand>(glm::vec2(0, -1), pOwner));
		input.BindKeyboardCommand('A'
			, KeyState::Down,
			std::make_shared<MoveCommand>(glm::vec2(-1, 0), pOwner));
		input.BindKeyboardCommand('S'
			, KeyState::Down,
			std::make_shared<MoveCommand>(glm::vec2(0, 1), pOwner));
		input.BindKeyboardCommand('D'
			, KeyState::Down,
			std::make_shared<MoveCommand>(glm::vec2(1, 0), pOwner));
	}

	void RemoveKeyboardMovement()
	{
		auto& input = InputManager::GetInstance();
		input.RemoveKeyboardBinding('W', KeyState::Down);
		input.RemoveKeyboardBinding('A', KeyState::Down);
		input.RemoveKeyboardBinding('S', KeyState::Down);
		input.RemoveKeyboardBinding('D', KeyState::Down);
	}
}
