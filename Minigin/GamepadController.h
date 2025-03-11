#pragma once

namespace dae
{
    class GamepadControllerImpl;
    class GamepadController
    {
    public:
        explicit GamepadController(int playerIndex);
        ~GamepadController();

        void Update();
        bool IsConnected() const;

        bool IsButtonDown(int buttonMask) const;
        bool WasButtonDown(int buttonMask) const;

        float GetLeftThumbX() const;
        float GetLeftThumbY() const;
        float GetRightThumbX() const;
        float GetRightThumbY() const;

        float GetLeftTrigger() const;
        float GetRightTrigger() const;

        void SetVibration(float leftMotor, float rightMotor);
    private:
        
        GamepadControllerImpl* m_pImpl;
    };
}

