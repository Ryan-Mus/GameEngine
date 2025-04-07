#pragma once
#include <string>

namespace dae
{ 
    enum class SoundEventType
    {
        LoadSound,
        PlaySound,
        StopSound,
        StopAllSounds,
        LoadMusic,
        PlayMusic,
        StopMusic,
        SetMasterVolume
    };
    
    struct SoundEvent
    {
        SoundEventType type;
        std::string id;
        std::string filePath;
        int loops = 0;
        int volume = 100;
    };
}
