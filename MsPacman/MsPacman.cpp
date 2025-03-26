#pragma once
#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
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


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_shared<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>("background.tga");
	scene.Add(background);

	auto titleFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto UIFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

	auto title = std::make_shared<dae::GameObject>(background.get());
	title->AddComponent<dae::TextComponent>("Programming 4 Assignment", titleFont);
	title->SetLocalPostion({ 80.f,20.f,0.f });
	scene.Add(title);

	auto logo = std::make_shared<dae::GameObject>(background.get());
	logo->AddComponent<dae::TextureComponent>("logo.tga");
	logo->SetLocalPostion({ 220, 180, 0.f });
	scene.Add(logo);

	auto infoKill = std::make_shared<dae::GameObject>(background.get());
	infoKill->AddComponent<dae::TextComponent>("Press 'C' to kill MsPacman or Button X to kill Pacman", UIFont);
	infoKill->SetLocalPostion({ 10.f, 400.f, 0.f });
	scene.Add(infoKill);

	auto infoPickUpBig = std::make_shared<dae::GameObject>(background.get());
	infoPickUpBig->AddComponent<dae::TextComponent>("Press 'X' or Button B to pick up a big pellet", UIFont);
	infoPickUpBig->SetLocalPostion({ 10.f, 420.f, 0.f });
	scene.Add(infoPickUpBig);

	auto infoPickUpSmall = std::make_shared<dae::GameObject>(background.get());
	infoPickUpSmall->AddComponent<dae::TextComponent>("Press 'Z' or Button A to pick up a small pellet", UIFont);
	infoPickUpSmall->SetLocalPostion({ 10.f, 440.f, 0.f });
	scene.Add(infoPickUpSmall);

	auto fps = std::make_shared<dae::GameObject>(background.get());
	fps->AddComponent<dae::TextComponent>(" ", titleFont);
	fps->AddComponent<dae::FpsComponent>();
	scene.Add(fps);

	//Pacman
	auto pacman = std::make_shared<dae::GameObject>();
	pacman->AddComponent<dae::TextureComponent>("spritesheet.png", 0, 0, 20, 20);
	pacman->AddComponent<dae::DieComponent>(std::make_unique<dae::Subject>());
	pacman->AddComponent<dae::PickUpPelletsComponent>(std::make_unique<dae::Subject>());
	dae::AddControllerMovement(200.f, 0, pacman.get());
	dae::AddControllerInteraction(0, pacman.get());
	pacman->SetLocalPostion({ 100.f,100.f,0.f });
	scene.Add(pacman);

	auto pacmanLives = std::make_shared<dae::GameObject>(pacman.get());
	pacmanLives->AddComponent<dae::TextComponent>("Lives: 3", UIFont);
	pacmanLives->AddComponent<dae::LivesUIComponent>();
	pacmanLives->SetLocalPostion({ 0.f,-20.f,0.f });
	scene.Add(pacmanLives);

	auto pacmanScore = std::make_shared<dae::GameObject>(pacman.get());
	pacmanScore->AddComponent<dae::TextComponent>("Score: 0", UIFont);
	pacmanScore->AddComponent<dae::ScoreUIcomponent>();
	pacmanScore->SetLocalPostion({ 0.f,-40.f,0.f });
	scene.Add(pacmanScore);

	//Add observers
	pacman->GetComponent<dae::DieComponent>()->AddObserver(pacmanLives->GetComponent<dae::LivesUIComponent>());
	pacman->GetComponent<dae::PickUpPelletsComponent>()->AddObserver(pacmanScore->GetComponent<dae::ScoreUIcomponent>());

	//ms Pacman
	auto msPacman = std::make_shared<dae::GameObject>();
	msPacman->AddComponent<dae::TextureComponent>("spritesheet.png", 80, 0, 20, 20);
	msPacman->AddComponent<dae::DieComponent>(std::make_unique<dae::Subject>());
	msPacman->AddComponent<dae::PickUpPelletsComponent>(std::make_unique<dae::Subject>());
	dae::AddKeyboardMovement(100.f, msPacman.get());
	dae::AddKeyboardInteraction(msPacman.get());
	msPacman->SetLocalPostion({ 200.f,100.f,0.f });
	scene.Add(msPacman);

	auto msPacmanLives = std::make_shared<dae::GameObject>(msPacman.get());
	msPacmanLives->AddComponent<dae::TextComponent>("Lives: 3", UIFont);
	msPacmanLives->AddComponent<dae::LivesUIComponent>();
	msPacmanLives->SetLocalPostion({ 0.f,-20.f,0.f });
	scene.Add(msPacmanLives);

	auto msPacmanScore = std::make_shared<dae::GameObject>(msPacman.get());
	msPacmanScore->AddComponent<dae::TextComponent>("Score: 0", UIFont);
	msPacmanScore->AddComponent<dae::ScoreUIcomponent>();
	msPacmanScore->SetLocalPostion({ 0.f,-40.f,0.f });
	scene.Add(msPacmanScore);

	//Add observers
	msPacman->GetComponent<dae::DieComponent>()->AddObserver(msPacmanLives->GetComponent<dae::LivesUIComponent>());
	msPacman->GetComponent<dae::PickUpPelletsComponent>()->AddObserver(msPacmanScore->GetComponent<dae::ScoreUIcomponent>());
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}