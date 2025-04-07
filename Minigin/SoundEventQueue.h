#pragma once
#include "SoundEvent.h"
#include <queue>
#include <mutex>
#include <condition_variable>

namespace dae
{
    class SoundEventQueue
    {
    public:
        void PushEvent(const SoundEvent& event);
        bool PopEvent(SoundEvent& event);
        bool HasEvents();
        void WaitForEvents();

    private:
        std::queue<SoundEvent> events;
        std::mutex eventMutex;
        std::condition_variable eventCondition;
    };
}

