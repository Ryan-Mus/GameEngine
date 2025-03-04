#include "ControllerMovementComponent.h"
#include "InputManager.h"
#include "MoveCommands.h"
#include "XInputDefines.h"

dae::ControllerMovementComponent::ControllerMovementComponent(float speed, int playerIndex, GameObject* pOwner)
	: Component{ pOwner }
{
	auto& input = InputManager::GetInstance();
	input.AddController(playerIndex);

	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN,
		playerIndex,
		KeyState::Down,
		std::make_shared<MoveDownCommand>(MoveDownCommand{ speed,pOwner }));
	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP,
		playerIndex,
		KeyState::Down,
		std::make_shared<MoveUpCommand>(MoveUpCommand{ speed,pOwner }));
	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT,
		playerIndex,
		KeyState::Down,
		std::make_shared<MoveLeftCommand>(MoveLeftCommand{ speed,pOwner }));
	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT,
		playerIndex,
		KeyState::Down,
		std::make_shared<MoveRightCommand>(MoveRightCommand{ speed,pOwner }));
}
