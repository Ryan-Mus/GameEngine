#pragma once
#include "InputManager.h"
#include <string>
#include "PlayerGhostCommands.h"
#include "XInputDefines.h"

void AddControllerGhostMovement(int playerIndex, GhostStateComponent* stateComp, const std::string& sceneName)
{
	auto& input = dae::InputManager::GetInstance();
	input.AddController(playerIndex);

	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_DOWN,
		playerIndex,
		dae::KeyState::Down,
		std::make_shared<PlayerGhostMoveCommand>(stateComp, glm::ivec2(0, 1)),
		sceneName);
	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_UP,
		playerIndex,
		dae::KeyState::Down,
		std::make_shared<PlayerGhostMoveCommand>(stateComp, glm::ivec2(0, -1)),
		sceneName);
	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_LEFT,
		playerIndex,
		dae::KeyState::Down,
		std::make_shared<PlayerGhostMoveCommand>(stateComp, glm::ivec2(-1, 0)),
		sceneName);
	input.BindControllerCommand(XINPUT_GAMEPAD_DPAD_RIGHT,
		playerIndex,
		dae::KeyState::Down,
		std::make_shared<PlayerGhostMoveCommand>(stateComp, glm::ivec2(1, 0)),
		sceneName);
}

void AddKeyboardGhostMovement(GhostStateComponent* stateComp, const std::string& sceneName)
{
	auto& input = dae::InputManager::GetInstance();

	input.BindKeyboardCommand('W',
		dae::KeyState::Down,
		std::make_shared<PlayerGhostMoveCommand>(stateComp, glm::ivec2(0, -1)),
		sceneName);
	input.BindKeyboardCommand('A',
		dae::KeyState::Down,
		std::make_shared<PlayerGhostMoveCommand>(stateComp, glm::ivec2(-1, 0)),
		sceneName);
	input.BindKeyboardCommand('S',
		dae::KeyState::Down,
		std::make_shared<PlayerGhostMoveCommand>(stateComp, glm::ivec2(0, 1)),
		sceneName);
	input.BindKeyboardCommand('D',
		dae::KeyState::Down,
		std::make_shared<PlayerGhostMoveCommand>(stateComp, glm::ivec2(1, 0)),
		sceneName);
}