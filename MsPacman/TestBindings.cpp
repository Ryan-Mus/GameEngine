#pragma once
#include "TestBindings.h"
#include "InputManager.h"
#include "XInputDefines.h"
#include "InteractionCommands.h"
namespace dae
{
    // Implementations
    void AddControllerInteraction(int playerIndex, GameObject* pOwner)
    {
        auto& input = InputManager::GetInstance();
        input.AddController(playerIndex);
        input.BindControllerCommand(XINPUT_GAMEPAD_X,
            playerIndex,
            KeyState::Pressed,
            std::make_shared<LoseLifeCommand>(pOwner));

		input.BindControllerCommand(XINPUT_GAMEPAD_B,
			playerIndex,
			KeyState::Pressed,
			std::make_shared<PickUpPelletsCommand>(true, pOwner));

		input.BindControllerCommand(XINPUT_GAMEPAD_A,
			playerIndex,
			KeyState::Pressed,
			std::make_shared<PickUpPelletsCommand>(false, pOwner));

    }

    void AddKeyboardInteraction(GameObject* pOwner)
    {
        auto& input = InputManager::GetInstance();
        input.BindKeyboardCommand('C',
            KeyState::Pressed,
            std::make_shared<LoseLifeCommand>(pOwner));

		input.BindKeyboardCommand('X',
			KeyState::Pressed,
			std::make_shared<PickUpPelletsCommand>(true, pOwner));

		input.BindKeyboardCommand('Z',
			KeyState::Pressed,
			std::make_shared<PickUpPelletsCommand>(false, pOwner));
    }
}