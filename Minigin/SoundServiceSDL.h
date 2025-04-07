#pragma once
#include "SoundService.h"
#include <memory>
namespace dae
{
    class SoundServiceSDL : public ISoundService
    {
    public:
        SoundServiceSDL();
        ~SoundServiceSDL() override;

        void LoadSound(const std::string& id, const std::string& filePath) override;
        void PlaySound(const std::string& id, int loops, int volume) override;
        void StopSound(const std::string& id) override;
        void StopAllSounds() override;

        void LoadMusic(const std::string& id, const std::string& filePath) override;
        void PlayMusic(const std::string& id, int loops, int volume) override;
        void StopMusic() override;

        void SetMasterVolume(int volume) override;

    private:
        class Impl;
        std::unique_ptr<Impl> pImpl;
    };
}

