#include "GamepadController.h"

// Using std::max instead of the max macro
#define NOMINMAX
#include <Windows.h>
#include <Xinput.h>
#include <memory>

namespace dae
{
    class GamepadControllerImpl
    {
    public:
        explicit GamepadControllerImpl(int playerIndex)
            : m_PlayerIndex(playerIndex), m_IsConnected(false)
        {
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
        }

        void Update()
        {
            memcpy(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
            ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
            DWORD result = XInputGetState(m_PlayerIndex, &m_CurrentState);
            m_IsConnected = (result == ERROR_SUCCESS);
        }

        bool IsConnected() const { return m_IsConnected; }
        bool IsButtonDown(int buttonMask) const { return (m_CurrentState.Gamepad.wButtons & buttonMask) != 0; }
        bool WasButtonDown(int buttonMask) const { return (m_PreviousState.Gamepad.wButtons & buttonMask) != 0; }

        float GetLeftThumbX() const { return NormalizeThumbstick(m_CurrentState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE); }
        float GetLeftThumbY() const { return NormalizeThumbstick(m_CurrentState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE); }
        float GetRightThumbX() const { return NormalizeThumbstick(m_CurrentState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE); }
        float GetRightThumbY() const { return NormalizeThumbstick(m_CurrentState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE); }

        float GetLeftTrigger() const { return NormalizeTrigger(m_CurrentState.Gamepad.bLeftTrigger); }
        float GetRightTrigger() const { return NormalizeTrigger(m_CurrentState.Gamepad.bRightTrigger); }

        void SetVibration(float leftMotor, float rightMotor)
        {
            leftMotor = std::max(0.0f, std::min(leftMotor, 1.0f));
            rightMotor = std::max(0.0f, std::min(rightMotor, 1.0f));

            WORD leftValue = static_cast<WORD>(leftMotor * 65535.0f);
            WORD rightValue = static_cast<WORD>(rightMotor * 65535.0f);

            XINPUT_VIBRATION vibration{};
            vibration.wLeftMotorSpeed = leftValue;
            vibration.wRightMotorSpeed = rightValue;
            XInputSetState(m_PlayerIndex, &vibration);
        }

    private:
        int m_PlayerIndex;
        bool m_IsConnected;
        XINPUT_STATE m_CurrentState;
        XINPUT_STATE m_PreviousState;

        float NormalizeThumbstick(short value, short deadzone) const
        {
            if (abs(value) < deadzone)
                return 0.0f;

            float normalized = (value < 0) ?
                (value + deadzone) / (32768.0f - deadzone) :
                (value - deadzone) / (32767.0f - deadzone);

            return std::max(-1.0f, std::min(normalized, 1.0f));
        }

        float NormalizeTrigger(BYTE value) const
        {
            if (value < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
                return 0.0f;

            return (value - XINPUT_GAMEPAD_TRIGGER_THRESHOLD) / (255.0f - XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
        }
    };

    GamepadController::GamepadController(int playerIndex)
        : m_pImpl(new dae::GamepadControllerImpl(playerIndex)) {
    }

    GamepadController::~GamepadController()
    {
        delete m_pImpl;
    }

    void GamepadController::Update() { m_pImpl->Update(); }
    bool GamepadController::IsConnected() const { return m_pImpl->IsConnected(); }
    bool GamepadController::IsButtonDown(int buttonMask) const { return m_pImpl->IsButtonDown(buttonMask); }
    bool GamepadController::WasButtonDown(int buttonMask) const { return m_pImpl->WasButtonDown(buttonMask); }
    float GamepadController::GetLeftThumbX() const { return m_pImpl->GetLeftThumbX(); }
    float GamepadController::GetLeftThumbY() const { return m_pImpl->GetLeftThumbY(); }
    float GamepadController::GetRightThumbX() const { return m_pImpl->GetRightThumbX(); }
    float GamepadController::GetRightThumbY() const { return m_pImpl->GetRightThumbY(); }
    float GamepadController::GetLeftTrigger() const { return m_pImpl->GetLeftTrigger(); }
    float GamepadController::GetRightTrigger() const { return m_pImpl->GetRightTrigger(); }
    void GamepadController::SetVibration(float leftMotor, float rightMotor) { m_pImpl->SetVibration(leftMotor, rightMotor); }

}