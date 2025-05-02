#pragma once
#include "SoundService.h"
#include <memory>

namespace dae
{
    class ServiceLocator
    {
    public:
        static ISoundService& GetSoundService()
        {
            return *soundService;
        }

        static void RegisterSoundService(std::unique_ptr<ISoundService> service)
        {
            soundService = std::move(service);
        }

        static void DestroySoundService()
        {
            soundService.reset();
        }

    private:
        static std::unique_ptr<ISoundService> soundService;
    };
}
