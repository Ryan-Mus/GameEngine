#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "FpsComponent.h"
#include "RotatorComponent.h"
#include "ThrashTheCache.h"
#include "TestBindings.h"
#include "MovementBinding.h"
#include "ScoreUIcomponent.h"
#include "LivesUIComponent.h"
#include "DieComponent.h"
#include "PickUpPelletsComponent.h"
#include "Scene.h"
#include "MsPacmanSubject.h"
#include "ServiceLocator.h"
#include "PacmanMovement.h"
#include "SpriteSheetAnimatorComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto UIFont = dae::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 24);
	std::string texturePath = "spritesheet.png";

	//ms Pacman
	auto msPacman = std::make_shared<dae::GameObject>();
	msPacman->AddComponent<dae::TextureComponent>(texturePath, 456, 0, 16, 16,3);
	msPacman->AddComponent<PacmanMovement>(200.f);
	dae::AddKeyboardMovement(msPacman.get());
	dae::AddControllerMovement(0, msPacman.get());
	msPacman->SetLocalPostion({ 200.f,100.f,0.f });

	auto& msPacmanAnimator = msPacman->AddComponent<dae::SpriteSheetAnimator>();
	//
	// Will later be read from a file
	//
	std::vector<dae::AnimationFrame> moveRightFrames = {
	{456, 0, 16, 16, 0.1f},    // First frame: x, y, width, height, duration
	{472, 0, 16, 16, 0.1f},   // Second frame
	{488, 0, 16, 16, 0.1f}   // Third frame
	};
	msPacmanAnimator.AddAnimation("Right", moveRightFrames, true);

	std::vector<dae::AnimationFrame> moveLeftFrames = {
	{456, 16, 16, 16, 0.1f},    // First frame: x, y, width, height, duration
	{472, 16, 16, 16, 0.1f},   // Second frame
	{488, 16, 16, 16, 0.1f}   // Third frame
	};
	msPacmanAnimator.AddAnimation("Left", moveLeftFrames, true);

	std::vector<dae::AnimationFrame> moveUpFrames = {
	{456, 32, 16, 16, 0.1f},    // First frame: x, y, width, height, duration
	{472, 32, 16, 16, 0.1f},   // Second frame
	{488, 32, 16, 16, 0.1f}   // Third frame
	};
	msPacmanAnimator.AddAnimation("Up", moveUpFrames, true);

	std::vector<dae::AnimationFrame> moveDownFrames = {
	{456, 48, 16, 16, 0.1f},    // First frame: x, y, width, height, duration
	{472, 48, 16, 16, 0.1f},   // Second frame
	{488, 48, 16, 16, 0.1f}   // Third frame
	};
	msPacmanAnimator.AddAnimation("Down", moveDownFrames, true);
	
	scene.Add(msPacman);

	//background
	auto background = std::make_shared<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>(texturePath, 228, 0, 224, 248,3);
	background->SetLocalPostion({ 0.f,72.f,0.f });
	scene.Add(background);

	auto highScore = std::make_shared <dae::GameObject>();
	highScore->AddComponent<dae::TextComponent>("High Score: 0", UIFont);
	highScore->SetLocalPostion({0,0,0});
	scene.Add(highScore);

	auto& soundManager = dae::ServiceLocator::GetSoundManager();
	soundManager.LoadSound("Intro", "Sounds/msstart.wav");
	soundManager.PlaySound("Intro", 0);

	soundManager.LoadSound("moving", "Sounds/mscoin.wav");
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}