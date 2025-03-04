#include "KeyboardMovementComponent.h"
#include "InputManager.h"
#include "MoveCommands.h"


dae::KeyboardMovementComponent::KeyboardMovementComponent(float speed, GameObject* pOwner)
	: Component{ pOwner }
{
	auto& input = InputManager::GetInstance();
	
	input.BindKeyboardCommand('W'
		, KeyState::Down,
		std::make_shared<MoveUpCommand>(MoveUpCommand{ speed,pOwner }));
	input.BindKeyboardCommand('A'
		,KeyState::Down,
		std::make_shared<MoveLeftCommand>(MoveLeftCommand{ speed,pOwner }));
	input.BindKeyboardCommand('S'
		, KeyState::Down,
		std::make_shared<MoveDownCommand>(MoveDownCommand{ speed,pOwner }));
	input.BindKeyboardCommand('D'
		, KeyState::Down,
		std::make_shared<MoveRightCommand>(MoveRightCommand{ speed,pOwner }));
}
