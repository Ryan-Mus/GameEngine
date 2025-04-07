#pragma once
#include "SoundService.h"
#include "SoundEventQueue.h"
#include <thread>
#include <atomic>
#include <memory>

namespace dae
{
    class SoundManager
    {
    public:
        void Init(const std::string& dataPath);
        SoundManager();
        ~SoundManager();

        // Client API
        void LoadSound(const std::string& id, const std::string& filePath);
        void PlaySound(const std::string& id, int loops = 0, int volume = 100);
        void StopSound(const std::string& id);
        void StopAllSounds();

        void LoadMusic(const std::string& id, const std::string& filePath);
        void PlayMusic(const std::string& id, int loops = -1, int volume = 100);
        void StopMusic();

        void SetMasterVolume(int volume);

    private:
        void WorkerThread();
        void ProcessEvent(const SoundEvent& event);

        std::unique_ptr<ISoundService> soundService;
        SoundEventQueue eventQueue;
        std::thread workerThread;
        std::atomic<bool> running;

		std::string m_DataPath;
    };
}

