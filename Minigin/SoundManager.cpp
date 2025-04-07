// ServiceLocator.h
// SoundManager.cpp
#include "SoundManager.h"
#include "SoundServiceSDL.h"
namespace dae
{
	void SoundManager::Init(const std::string& dataPath)
	{
		m_DataPath = dataPath;
	}
	SoundManager::SoundManager()
        : soundService(std::make_unique<SoundServiceSDL>()),
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
            SoundEvent event;
            eventQueue.WaitForEvents();

            // Process all queued events
            while (eventQueue.PopEvent(event))
            {
                if (!running) break;
                ProcessEvent(event);
            }
        }
    }

    void SoundManager::ProcessEvent(const SoundEvent& event)
    {
        switch (event.type)
        {
        case SoundEventType::LoadSound:
            soundService->LoadSound(event.id, event.filePath);
            break;
        case SoundEventType::PlaySound:
            soundService->PlaySound(event.id, event.loops, event.volume);
            break;
            // Handle other event types...
        }
    }
}