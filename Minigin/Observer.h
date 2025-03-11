#pragma once
namespace dae
{
    enum class EventType
    {
        PlayerDied,
        Score10,
        Score100
    };

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void OnNotify(EventType event) = 0;
    };

}

