#pragma once
#include <string>
namespace dae
{
    class ISoundService
    {
    public:
        virtual ~ISoundService() = default;

        virtual void LoadSound(const std::string& id, const std::string& filePath) = 0;
        virtual void PlaySound(const std::string& id, int loops = 0, int volume = 100) = 0;
        virtual void StopSound(const std::string& id) = 0;
        virtual void StopAllSounds() = 0;

        virtual void LoadMusic(const std::string& id, const std::string& filePath) = 0;
        virtual void PlayMusic(const std::string& id, int loops = -1, int volume = 100) = 0;
        virtual void StopMusic() = 0;

        virtual void SetMasterVolume(int volume) = 0;
    };
}