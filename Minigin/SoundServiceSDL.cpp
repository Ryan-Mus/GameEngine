#include "SoundServiceSDL.h"
#include <SDL_mixer.h>
#include <unordered_map>
#include <iostream>
#include <algorithm>

namespace dae
{
    class SoundServiceSDL::Impl
    {
    public:
        Impl()
        {
            int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
            if (Mix_Init(flags) != flags)
            {
                std::cerr << "Failed to initialize SDL_mixer! Error: " << Mix_GetError() << std::endl;
            }

            if (Mix_OpenAudioDevice(44100, MIX_DEFAULT_FORMAT, 2, 2048, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE) < 0)
            {
                std::cerr << "SDL_mixer could not open audio! Error: " << Mix_GetError() << std::endl;
            }
        }


        ~Impl()
        {
            for (auto& [id, chunk] : soundEffects)
            {
                Mix_FreeChunk(chunk);
            }
            soundEffects.clear();

            for (auto& [id, music] : musicTracks)
            {
                Mix_FreeMusic(music);
            }
            musicTracks.clear();

            Mix_CloseAudio();
			Mix_Quit();
        }

        std::unordered_map<std::string, Mix_Chunk*> soundEffects;
        std::unordered_map<std::string, Mix_Music*> musicTracks;
        Mix_Music* currentMusic = nullptr;
        int masterVolume = MIX_MAX_VOLUME;
    };

    SoundServiceSDL::SoundServiceSDL() : pImpl(std::make_unique<Impl>()) {}
	SoundServiceSDL::~SoundServiceSDL() = default;

    void SoundServiceSDL::LoadSound(const std::string& id, const std::string& filePath)
    {
        Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
        if (!chunk)
        {
            std::cerr << "Failed to load sound effect: " << filePath << " SDL_mixer Error: " << Mix_GetError() << std::endl;
            return;
        }

        pImpl->soundEffects[id] = chunk;
    }

    void SoundServiceSDL::PlaySound(const std::string& id, int loops, int volume)
    {
        auto it = pImpl->soundEffects.find(id);
        if (it == pImpl->soundEffects.end())
        {
            std::cerr << "Sound effect not found: " << id << std::endl;
            return;
        }

        int scaledVolume = (volume * pImpl->masterVolume) / 100;
        Mix_VolumeChunk(it->second, scaledVolume);
        Mix_PlayChannel(-1, it->second, loops);
    }


    void SoundServiceSDL::StopSound(const std::string& id)
    {
    	auto it = pImpl->soundEffects.find(id);
    	if (it == pImpl->soundEffects.end())
    	{
    		std::cerr << "Sound effect not found: " << id << std::endl;
    		return;
    	}
    	Mix_HaltChannel(-1); // Stop all channels
    }
    
    void SoundServiceSDL::StopAllSounds()
    {
    	Mix_HaltChannel(-1); // Stop all channels
    }
    
    void SoundServiceSDL::LoadMusic(const std::string& id, const std::string& filePath)
    {
    	Mix_Music* music = Mix_LoadMUS(filePath.c_str());
    	if (!music)
    	{
    		std::cerr << "Failed to load music: " << filePath << " SDL_mixer Error: " << Mix_GetError() << std::endl;
    		return;
    	}
    	pImpl->musicTracks[id] = music;
    }
    
    void SoundServiceSDL::PlayMusic(const std::string& id, int loops, int volume)
    {
    	auto it = pImpl->musicTracks.find(id);
    	if (it == pImpl->musicTracks.end())
    	{
    		std::cerr << "Music track not found: " << id << std::endl;
    		return;
    	}
    	int scaledVolume = (volume * pImpl->masterVolume) / 100;
    	Mix_VolumeMusic(scaledVolume);
    	if (pImpl->currentMusic != it->second)
    	{
    		Mix_PlayMusic(it->second, loops);
    		pImpl->currentMusic = it->second;
    	}
    }
    
    void SoundServiceSDL::StopMusic()
    {
    	Mix_HaltMusic();
    	pImpl->currentMusic = nullptr;
    }
    
    void SoundServiceSDL::SetMasterVolume(int volume)
    {
    	pImpl->masterVolume = std::clamp(volume, 0, MIX_MAX_VOLUME);
    	Mix_Volume(-1, pImpl->masterVolume); // Set volume for all channels
    	Mix_VolumeMusic(pImpl->masterVolume); // Set volume for music
    }
}
