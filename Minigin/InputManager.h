#pragma once
//No XInput or Windows or SDL in these headers
#include <vector>
#include <memory>

#include "GamepadController.h"
#include "Command.h"
#include "Singleton.h"

namespace dae
{
    enum class KeyState
    {
        Pressed,    // Key was just pressed this frame
        Released,   // Key was just released this frame
        Down,        // Key is being held down
        Up
    };

    enum class InputType
    {
        Keyboard,
        Controller
    };

    struct InputBinding
    {
        InputType type;
        int keyCode;       // Keyboard key or controller button
        int playerIndex;   // Controller index (0-3)
        KeyState keyState; // The state that triggers the command
        std::shared_ptr<Command> command;
    };

    class InputManager final: public Singleton<InputManager> 
    {
    public:
        InputManager();
        ~InputManager();

        bool ProcessInput();

        // Bind keyboard key to a command
        void BindKeyboardCommand(int keyCode, KeyState state, std::shared_ptr<Command> command);

        // Bind controller button to a command
        void BindControllerCommand(int buttonId, int playerIndex, KeyState state, std::shared_ptr<Command> command);

        void RemoveKeyboardBinding(int keyCode, KeyState state);
        void RemoveControllerBinding(int buttonId, int playerIndex, KeyState state);
        void RemoveAllBindings();
        void RemoveAllKeyboardBindings();
        void RemoveAllControllerBindings(int playerIndex = -1); // -1 means all controllers

        // Remove a specific command binding
        void RemoveCommandBinding(std::shared_ptr<Command> command);

        void AddController(int playerIndex);
        void RemoveController(int playerIndex);
        GamepadController* GetController(int playerIndex);

    private:
        std::vector<std::unique_ptr<dae::GamepadController>> m_Controllers;
        // Maps for storing input bindings
        std::vector<InputBinding> m_InputBindings;

        // Previous frame state for detecting transitions
        bool m_PreviousKeyboardState[256];

        // Current frame state
        bool m_CurrentKeyboardState[256];

        // Helper methods
        bool IsKeyDown(int keyCode);
        bool WasKeyDown(int keyCode);

        // Get key state (Pressed, Released, Down)
        KeyState GetKeyState(int keyCode);
        KeyState GetButtonState(int buttonId, int playerIndex);

        std::vector<InputBinding>::iterator FindBinding(InputType type, int keyCode, KeyState state, int playerIndex = 0);
    };
}
