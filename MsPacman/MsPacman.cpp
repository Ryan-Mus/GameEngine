#pragma once
#include <string>
#include <memory>
#include "Minigin.h"
#include "GameLoader.h"
#include "SoundManager.h"
#include "SoundServiceSDL.h"
#include "ServiceLocator.h"
#include "SceneManager.h"

void load()
{
	auto& gameLoader = GameLoader::GetInstance();
	gameLoader.loadGameJSON("../Data/MainMenu.json");
	

	auto soundManager = std::make_unique<dae::SoundManager>();
	soundManager->Init("../Data/");
	// Set the internal sound service implementation
	soundManager->SetSoundServiceImpl(std::make_unique<dae::SoundServiceSDL>());

	// Register the sound manager with the service locator
	dae::ServiceLocator::RegisterSoundService(std::move(soundManager));

	dae::ServiceLocator::GetSoundService().LoadSound("MsPacman", "Sounds/msstart.wav");
	dae::ServiceLocator::GetSoundService().LoadSound("eatPellet", "Sounds/msdot.wav");
	dae::ServiceLocator::GetSoundService().LoadSound("eatBigPellet", "Sounds/msghostblue.wav");
	dae::ServiceLocator::GetSoundService().LoadSound("Ghost1", "Sounds/msghost1.wav");

	dae::ServiceLocator::GetSoundService().PlaySound("MsPacman");
	dae::ServiceLocator::GetSoundService().SetMasterVolume(32);

	dae::SceneManager::GetInstance().SetActiveScene("MainMenu");
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}