#include "SoundManager.h"
#include <memory>

namespace dae
{
    class ServiceLocator
    {
    public:
        static SoundManager& GetSoundManager()
        {
            if (!soundManager)
                soundManager = std::make_unique<SoundManager>();
            return *soundManager;
        }

        static void DestroySoundManager()
        {
            soundManager.reset();
        }

    private:
        static std::unique_ptr<SoundManager> soundManager;
    };
}