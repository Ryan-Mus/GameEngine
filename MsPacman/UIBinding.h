#pragma once
#include "InputManager.h"
#include "UIInteractionCommands.h"
#include "XInputDefines.h"

void AddUIKeyboardBinding(dae::GameObject* pButtonManager)
{
	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand('W'
		, dae::KeyState::Down,
		std::make_shared<dae::SelectPreviousButtonCommand>(pButtonManager));

	input.BindKeyboardCommand('S'
		, dae::KeyState::Down,
		std::make_shared<dae::SelectNextButtonCommand>(pButtonManager));

	input.BindKeyboardCommand(XINPUT_KEYBOARD_ENTER
		, dae::KeyState::Down,
		std::make_shared<dae::PressSelectedButtonCommand>(pButtonManager));
}
