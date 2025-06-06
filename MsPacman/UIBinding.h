#pragma once
#include "InputManager.h"
#include "UIInteractionCommands.h"
#include "XInputDefines.h"

void AddUIKeyboardBinding(dae::GameObject* pButtonManager)
{
	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand('W'
		, dae::KeyState::Pressed,
		std::make_shared<dae::SelectPreviousButtonCommand>(pButtonManager));

	input.BindKeyboardCommand('S'
		, dae::KeyState::Pressed,
		std::make_shared<dae::SelectNextButtonCommand>(pButtonManager));

	input.BindKeyboardCommand(XINPUT_KEYBOARD_ENTER
		, dae::KeyState::Pressed,
		std::make_shared<dae::PressSelectedButtonCommand>(pButtonManager));
}

void AddUIControllerBinding(int playerId, dae::GameObject* pButtonManager)
{
	auto& input = dae::InputManager::GetInstance();

	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP, playerId,
		dae::KeyState::Pressed,
		std::make_shared<dae::SelectPreviousButtonCommand>(pButtonManager));

	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, playerId,
		dae::KeyState::Pressed,
		std::make_shared<dae::SelectNextButtonCommand>(pButtonManager));

	input.BindControllerCommand(XINPUT_GAMEPAD_A, playerId,
		dae::KeyState::Pressed,
		std::make_shared<dae::PressSelectedButtonCommand>(pButtonManager));
}
