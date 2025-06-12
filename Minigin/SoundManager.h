#pragma once
#include "SoundService.h"
#include "SoundEventQueue.h"
#include <thread>
#include <atomic>
#include <memory>

namespace dae
{
    class SoundManager final : public ISoundService
    {
    public:
        SoundManager();
        ~SoundManager();

        SoundManager(const SoundManager&) = delete;
        SoundManager(SoundManager&&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;
        SoundManager& operator=(SoundManager&&) = delete;

        void Init(const std::string& dataPath);

        // ISoundService interface
        void LoadSound(const std::string& id, const std::string& filePath) override;
        void PlaySound(const std::string& id, int loops = 0, int volume = 100) override;
        void StopSound(const std::string& id) override;
        void StopAllSounds() override;

        void LoadMusic(const std::string& id, const std::string& filePath) override;
        void PlayMusic(const std::string& id, int loops = -1, int volume = 100) override;
        void StopMusic() override;

        void SetMasterVolume(int volume) override;
        int  GetMasterVolume() override;

        // New method to set the internal sound service
        void SetSoundServiceImpl(std::unique_ptr<ISoundService> service);

    private:
        void WorkerThread();
        void ProcessEvent(const SoundEvent& event);

        std::unique_ptr<ISoundService> soundServiceImpl;
        SoundEventQueue eventQueue;
        std::thread workerThread;
        std::atomic<bool> running;

        std::string m_DataPath;
    };
}

