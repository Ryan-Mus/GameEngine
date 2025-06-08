#pragma once
#include "InputManager.h"
#include "HighscoreCommands.h"
#include "UIInteractionCommands.h"
#include "XInputDefines.h"

void AddHighscoreKeyboardBinding(dae::GameObject* pButtonManager, dae::GameObject* pHighscoreManager, const std::string& sceneName = "")
{
	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand('A'
		, dae::KeyState::Pressed,
		std::make_shared<dae::SelectPreviousButtonCommand>(pButtonManager),
		sceneName);

	input.BindKeyboardCommand('D'
		, dae::KeyState::Pressed,
		std::make_shared<dae::SelectNextButtonCommand>(pButtonManager),
		sceneName);

	input.BindKeyboardCommand(XINPUT_KEYBOARD_ENTER
		, dae::KeyState::Pressed,
		std::make_shared<dae::PressSelectedButtonCommand>(pButtonManager),
		sceneName);

	input.BindKeyboardCommand('W'
		, dae::KeyState::Pressed,
		std::make_shared<ChangeLetterCommand>(pHighscoreManager, true),
		sceneName);

	input.BindKeyboardCommand('S'
		, dae::KeyState::Pressed,
		std::make_shared<ChangeLetterCommand>(pHighscoreManager, false),
		sceneName);
}

void AddHighScoreControllerBinding(int playerId, dae::GameObject* pButtonManager, dae::GameObject* pHighscoreManager,const std::string& sceneName = "")
{
	auto& input = dae::InputManager::GetInstance();

	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT, playerId,
		dae::KeyState::Pressed,
		std::make_shared<dae::SelectPreviousButtonCommand>(pButtonManager),
		sceneName);

	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT, playerId,
		dae::KeyState::Pressed,
		std::make_shared<dae::SelectNextButtonCommand>(pButtonManager),
		sceneName);

	input.BindControllerCommand(XINPUT_GAMEPAD_A, playerId,
		dae::KeyState::Pressed,
		std::make_shared<dae::PressSelectedButtonCommand>(pButtonManager),
		sceneName);

	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP, playerId,
		dae::KeyState::Pressed,
		std::make_shared<ChangeLetterCommand>(pHighscoreManager, true),
		sceneName);

	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN, playerId,
		dae::KeyState::Pressed,
		std::make_shared<ChangeLetterCommand>(pHighscoreManager, false),
		sceneName);
}
