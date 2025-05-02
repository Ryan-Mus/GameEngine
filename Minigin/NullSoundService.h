#pragma once
#include "SoundService.h"

namespace dae
{
    class NullSoundService final : public ISoundService
    {
    public:
        void LoadSound(const std::string&, const std::string&) override {}
        void PlaySound(const std::string&, int, int) override {}
        void StopSound(const std::string&) override {}
        void StopAllSounds() override {}

        void LoadMusic(const std::string&, const std::string&) override {}
        void PlayMusic(const std::string&, int, int) override {}
        void StopMusic() override {}

        void SetMasterVolume(int) override {}
    };
}
