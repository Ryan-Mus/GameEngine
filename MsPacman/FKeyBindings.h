#pragma once
#include <InputManager.h>
#include "FKeyCommands.h"
#include <XInputDefines.h>

void AddSoundBindings()
{
	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand(XINPUT_KEYBOARD_F2
		, dae::KeyState::Pressed,
		std::make_shared<MuteCommand>());
}

void AddSkipLevelBinding(dae::GameObject* pGrid, const std::string& sceneName = "")
{
	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand(XINPUT_KEYBOARD_F1
		,dae::KeyState::Pressed,
		std::make_shared<SkipLevelCommand>(pGrid),
		sceneName);
}