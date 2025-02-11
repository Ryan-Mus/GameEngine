#pragma once
#include <chrono>
#include <thread>
#include "Singleton.h"

class Time final: public dae::Singleton<Time>
{
public:
    void Update()
    {
        auto current_time = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(current_time - lastTime).count();
        lastTime = current_time;
        lag += deltaTime;
    }

    void AdvanceStep()
    {
        lag -= fixedTimeStep;
    }

    bool NeedsStep() const
    {
        return lag >= fixedTimeStep;
    }

    float GetDeltaTime() const { return deltaTime; }
    float GetFixedTimeStep() const { return fixedTimeStep; }

    Time() : lastTime(std::chrono::high_resolution_clock::now()), deltaTime(0.0f), lag(0.0f) {}
    ~Time() = default;

private:
    std::chrono::high_resolution_clock::time_point lastTime;
    float deltaTime;
    float lag;
    const float fixedTimeStep = 0.008f; // 8ms
};

