// SoundManager.cpp
#include "SoundManager.h"
#include "SoundServiceSDL.h"
#include "NullSoundService.h"

namespace dae
{
    SoundManager::SoundManager()
        : soundServiceImpl(std::make_unique<NullSoundService>()), // Default to NullSoundService
        running(true)
    {
        workerThread = std::thread(&SoundManager::WorkerThread, this);
    }

    SoundManager::~SoundManager()
    {
        running = false;
        eventQueue.PushEvent({}); // Push empty event to wake up thread
        if (workerThread.joinable())
            workerThread.join();
    }

    void SoundManager::Init(const std::string& dataPath)
    {
        m_DataPath = dataPath;
    }

    void SoundManager::SetSoundServiceImpl(std::unique_ptr<ISoundService> service)
    {
        soundServiceImpl = std::move(service);
    }

    // ISoundService interface implementations
    void SoundManager::LoadSound(const std::string& id, const std::string& filePath)
    {
        SoundEvent event;
        event.type = SoundEventType::LoadSound;
        event.id = id;
        event.filePath = m_DataPath + filePath;
        eventQueue.PushEvent(event);
    }

    void SoundManager::PlaySound(const std::string& id, int loops, int volume)
    {
        SoundEvent event;
        event.type = SoundEventType::PlaySound;
        event.id = id;
        event.loops = loops;
        event.volume = volume;
        eventQueue.PushEvent(event);
    }

    void SoundManager::StopSound(const std::string& id)
    {
        SoundEvent event;
        event.type = SoundEventType::StopSound;
        event.id = id;
        eventQueue.PushEvent(event);
    }

    void SoundManager::StopAllSounds()
    {
        SoundEvent event;
        event.type = SoundEventType::StopAllSounds;
        eventQueue.PushEvent(event);
    }

    void SoundManager::LoadMusic(const std::string& id, const std::string& filePath)
    {
        SoundEvent event;
        event.type = SoundEventType::LoadMusic;
        event.id = id;
        event.filePath = m_DataPath + filePath;
        eventQueue.PushEvent(event);
    }

    void SoundManager::PlayMusic(const std::string& id, int loops, int volume)
    {
        SoundEvent event;
        event.type = SoundEventType::PlayMusic;
        event.id = id;
        event.loops = loops;
        event.volume = volume;
        eventQueue.PushEvent(event);
    }

    void SoundManager::StopMusic()
    {
        SoundEvent event;
        event.type = SoundEventType::StopMusic;
        eventQueue.PushEvent(event);
    }

    void SoundManager::SetMasterVolume(int volume)
    {
        SoundEvent event;
        event.type = SoundEventType::SetMasterVolume;
        event.volume = volume;
        eventQueue.PushEvent(event);
    }

    void SoundManager::WorkerThread()
    {
        while (running)
        {
            eventQueue.WaitForEvents();

            // Process all queued events
            SoundEvent event;
            while (eventQueue.PopEvent(event))
            {
                if (!running)
                    break;
                ProcessEvent(event);
            }
        }
    }

    void SoundManager::ProcessEvent(const SoundEvent& event)
    {
        switch (event.type)
        {
        case SoundEventType::LoadSound:
            soundServiceImpl->LoadSound(event.id, event.filePath);
            break;
        case SoundEventType::PlaySound:
            soundServiceImpl->PlaySound(event.id, event.loops, event.volume);
            break;
        case SoundEventType::StopSound:
            soundServiceImpl->StopSound(event.id);
            break;
        case SoundEventType::StopAllSounds:
            soundServiceImpl->StopAllSounds();
            break;
        case SoundEventType::LoadMusic:
            soundServiceImpl->LoadMusic(event.id, event.filePath);
            break;
        case SoundEventType::PlayMusic:
            soundServiceImpl->PlayMusic(event.id, event.loops, event.volume);
            break;
        case SoundEventType::StopMusic:
            soundServiceImpl->StopMusic();
            break;
        case SoundEventType::SetMasterVolume:
            soundServiceImpl->SetMasterVolume(event.volume);
            break;
        default:
            // Handle unknown event types if necessary
            break;
        }
    }
}
