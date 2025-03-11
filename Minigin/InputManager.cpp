#include <SDL.h>
#include <Windows.h>
#include <Xinput.h>
#include <imgui_impl_sdl2.h>
#include <cstring>
#include "InputManager.h"

dae::InputManager::InputManager()
{
    memset(m_PreviousKeyboardState, 0, sizeof(m_PreviousKeyboardState));
    memset(m_CurrentKeyboardState, 0, sizeof(m_CurrentKeyboardState));

    m_Controllers.resize(XUSER_MAX_COUNT);
}

dae::InputManager::~InputManager() {}

bool dae::InputManager::ProcessInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;     
        }
        ImGui_ImplSDL2_ProcessEvent(&e);
    }
    // Store previous frame state
    memcpy(m_PreviousKeyboardState, m_CurrentKeyboardState, sizeof(m_CurrentKeyboardState));

    // Update keyboard state (Win32 API)
    for (int i = 0; i < 256; i++) {
        m_CurrentKeyboardState[i] = GetAsyncKeyState(i) & 0x8000;
    }

    // Update controllers state
    for (int i = 0; i < XUSER_MAX_COUNT; i++)
    {
        if (m_Controllers[i])
        {
            m_Controllers[i]->Update();
        }
    }

    // Execute commands based on input state
    for (const auto& binding : m_InputBindings)
    {
        bool shouldExecute = false;

        if (binding.type == InputType::Keyboard)
        {
            KeyState state = GetKeyState(binding.keyCode);
            shouldExecute = (state == binding.keyState);
        }
        else if (binding.type == InputType::Controller)
        {
            // Use the controller wrapper
            GamepadController* controller = m_Controllers[binding.playerIndex].get();
            if (controller && controller->IsConnected())
            {
                KeyState state = GetButtonState(binding.keyCode, binding.playerIndex);
                shouldExecute = (state == binding.keyState);
            }
        }

        if (shouldExecute && binding.command)
        {
            binding.command->Execute();
        }
    }

    return true;
}


void dae::InputManager::BindKeyboardCommand(int keyCode, KeyState state, std::shared_ptr<Command> command)
{
    InputBinding binding;
    binding.type = InputType::Keyboard;
    binding.keyCode = keyCode;
    binding.keyState = state;
    binding.command = command;
    binding.playerIndex = 0; // Not used for keyboard

    m_InputBindings.push_back(binding);
}

void dae::InputManager::BindControllerCommand(int buttonId, int playerIndex, KeyState state, std::shared_ptr<Command> command)
{
    InputBinding binding;
    binding.type = InputType::Controller;
    binding.keyCode = buttonId;
    binding.playerIndex = playerIndex;
    binding.keyState = state;
    binding.command = command;

    m_InputBindings.push_back(binding);
}

void dae::InputManager::AddController(int playerIndex)
{
    if (playerIndex >= 0 && playerIndex < XUSER_MAX_COUNT)
    {
       m_Controllers[playerIndex] = std::make_unique<GamepadController>(playerIndex);
    }
}

void dae::InputManager::RemoveController(int playerIndex)
{
    if (playerIndex >= 0 && playerIndex < XUSER_MAX_COUNT)
    {
        m_Controllers[playerIndex].reset();
    }
}

dae::GamepadController* dae::InputManager::GetController(int playerIndex)
{
    if (playerIndex >= 0 && playerIndex < XUSER_MAX_COUNT)
    {
        return m_Controllers[playerIndex].get();
    }
    return nullptr;
}

bool dae::InputManager::IsKeyDown(int keyCode)
{
    return m_CurrentKeyboardState[keyCode];
}

bool dae::InputManager::WasKeyDown(int keyCode)
{
    return m_PreviousKeyboardState[keyCode];
}


dae::KeyState dae::InputManager::GetKeyState(int keyCode)
{
    bool isDown = IsKeyDown(keyCode);
    bool wasDown = WasKeyDown(keyCode);

    if (isDown && !wasDown)
        return KeyState::Pressed;
    else if (!isDown && wasDown)
        return KeyState::Released;
    else if (isDown && wasDown)
        return KeyState::Down;

    return KeyState::Released; // Default
}

dae::KeyState dae::InputManager::GetButtonState(int buttonId, int playerIndex)
{
    GamepadController* controller = m_Controllers[playerIndex].get();
    if (!controller || !controller->IsConnected())
        return KeyState::Up;

    bool isDown = controller->IsButtonDown(buttonId);
    bool wasDown = controller->WasButtonDown(buttonId);

    if (isDown && !wasDown)
        return KeyState::Pressed;
    else if (!isDown && wasDown)
        return KeyState::Released;
    else if (isDown && wasDown)
        return KeyState::Down;
    // if !isDown and !wasDown = up
    return KeyState::Up;
}

std::vector<dae::InputBinding>::iterator dae::InputManager::FindBinding(InputType type, int keyCode,KeyState state, int playerIndex)
{
    return std::find_if(m_InputBindings.begin(), m_InputBindings.end(),
        [type, keyCode, state, playerIndex](const InputBinding& binding) -> bool 
        {
            return binding.type == type &&
                binding.keyCode == keyCode &&
                binding.keyState == state &&
                (type == InputType::Keyboard || binding.playerIndex == playerIndex);
        });
}

void dae::InputManager::RemoveKeyboardBinding(int keyCode, KeyState state)
{
    auto it = FindBinding(InputType::Keyboard, keyCode, state);
    if (it != m_InputBindings.end()) 
    {
        m_InputBindings.erase(it);
    }
}

void dae::InputManager::RemoveControllerBinding(int buttonId, int playerIndex, KeyState state)
{
    auto it = FindBinding(InputType::Controller, buttonId, state, playerIndex);
    if (it != m_InputBindings.end()) 
    {
        m_InputBindings.erase(it);
    }
}

void dae::InputManager::RemoveAllBindings()
{
    m_InputBindings.clear();
}

void dae::InputManager::RemoveAllKeyboardBindings()
{
    m_InputBindings.erase(
        std::remove_if(m_InputBindings.begin(), m_InputBindings.end(),
            [](const InputBinding& binding) 
            {
                return binding.type == InputType::Keyboard;
            }),
        m_InputBindings.end());
}

void dae::InputManager::RemoveAllControllerBindings(int playerIndex)
{
    if (playerIndex >= 0) 
    {
        // Remove bindings for specific controller
        m_InputBindings.erase(
            std::remove_if(m_InputBindings.begin(), m_InputBindings.end(),
                [playerIndex](const InputBinding& binding) 
                {
                    return binding.type == InputType::Controller &&
                        binding.playerIndex == playerIndex;
                }),
            m_InputBindings.end());
    }
    else 
    {
        // Remove all controller bindings
        m_InputBindings.erase(
            std::remove_if(m_InputBindings.begin(), m_InputBindings.end(),
                [](const InputBinding& binding) {
                    return binding.type == InputType::Controller;
                }),
            m_InputBindings.end());
    }
}

void dae::InputManager::RemoveCommandBinding(std::shared_ptr<Command> command)
{
    //Goes through all bindings and removes the ones with a specific command
    m_InputBindings.erase(
        std::remove_if(m_InputBindings.begin(), m_InputBindings.end(),
            [command](const InputBinding& binding) 
            {
                return binding.command == command;
            }),
        m_InputBindings.end());
}
