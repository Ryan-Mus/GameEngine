#include "SoundEventQueue.h"
namespace dae
{
    void SoundEventQueue::PushEvent(const SoundEvent& event)
    {
        {
            std::lock_guard<std::mutex> lock(eventMutex);
            events.push(event);
        }
        eventCondition.notify_one();
    }

    bool SoundEventQueue::PopEvent(SoundEvent& event)
    {
        std::lock_guard<std::mutex> lock(eventMutex);
        if (events.empty())
            return false;

        event = events.front();
        events.pop();
        return true;
    }

    bool SoundEventQueue::HasEvents()
    {
        std::lock_guard<std::mutex> lock(eventMutex);
        return !events.empty();
    }

    void SoundEventQueue::WaitForEvents()
    {
        std::unique_lock<std::mutex> lock(eventMutex);
        eventCondition.wait(lock, [this] { return !events.empty(); });
    }
}
