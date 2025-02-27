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
#include "ImGuiComponent.h"
#include "Scene.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_shared<dae::GameObject>();
	background->AddComponent<dae::TextureComponent>("background.tga");
	scene.Add(background);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto title = std::make_shared<dae::GameObject>(background.get());
	title->AddComponent<dae::TextComponent>("Programming 4 Assignment", font);
	title->SetLocalPostion({ 80.f,20.f,0.f });
	scene.Add(title);

	auto logo = std::make_shared<dae::GameObject>(background.get());
	logo->AddComponent<dae::TextureComponent>("logo.tga");
	logo->SetLocalPostion({ 220, 180, 0.f });
	scene.Add(logo);

	auto fps = std::make_shared<dae::GameObject>(background.get());
	fps->AddComponent<dae::TextComponent>(" ", font);
	fps->AddComponent<dae::FpsComponent>();
	scene.Add(fps);

	auto pos = std::make_shared<dae::GameObject>();
	pos->SetLocalPostion({ 200.f,200.f,0.f });
	scene.Add(pos);
	
	//Parent in initialiser
	auto pacman = std::make_shared<dae::GameObject>(pos.get());
	pacman->AddComponent<dae::TextureComponent>("spritesheet.png",0,0,20,20);
	pacman->AddComponent<dae::RotatorComponent>(3.14f);
	pacman->SetLocalPostion({ 30.f,0.f,0.f });
	scene.Add(pacman);

	//Parent with function
	auto msPacman = std::make_shared<dae::GameObject>();
	msPacman->SetParent(pacman.get());
	msPacman->AddComponent<dae::TextureComponent>("spritesheet.png",80, 0, 20, 20);
	msPacman->AddComponent<dae::RotatorComponent>(-6.28f);
	msPacman->SetLocalPostion({ 50.f,0.f,0.f });
	scene.Add(msPacman);

	auto imGUI = std::make_shared<dae::GameObject>();
	imGUI->AddComponent<dae::ImGuiComponent>();
	scene.Add(imGUI);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}