#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "json.hpp"

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
#include "GhostMovement.h"
#include "GhostStateComponent.h"
#include "ChaseState.h"
#include "SpriteSheetAnimatorComponent.h"
#include "PacmanGrid.h"
#include "SoundManager.h"
#include "SoundServiceSDL.h"

#include  "CustomPacmanDefines.h"


using ComponentGetter = std::function<dae::Component* (dae::GameObject*)>;

std::unordered_map<std::string, ComponentGetter> componentGetters = {
	{ "pacmanGrid", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<PacmanGrid>(); } },
	{ "scoreUIComponent", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<dae::ScoreUIcomponent>(); } }
	// Add other components as needed
};

dae::Component* GetComponentByName(dae::GameObject* gameObject, const std::string& componentName)
{
	auto it = componentGetters.find(componentName);
	if (it != componentGetters.end())
	{
		return it->second(gameObject);
	}
	return nullptr;
}

void AddObserverToSubject(dae::Component* subjectComponent, dae::Component* observerComponent)
{
	// For this example, we assume MsPacmanSubject and MsPacmanObserver are the base classes
	auto subject = dynamic_cast<MsPacmanSubject*>(subjectComponent);
	auto observer = dynamic_cast<MsPacmanObserver*>(observerComponent);

	if (subject && observer)
	{
		subject->AddObserver(observer);
	}
	else
	{
		throw std::runtime_error("Failed to cast components to subject or observer.");
	}
}

void load()
{
	auto Font = dae::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 24);
	std::string texturePath = "spritesheet.png";

	std::ifstream file("../Data/game.json");
	nlohmann::json sceneData;
	file >> sceneData;

	// Map to store game objects by name for parent relationships
	std::unordered_map<std::string, std::shared_ptr<dae::GameObject>> gameObjectMap;

	// Iterate over scenes
	for (const auto& sceneJson : sceneData["scenes"])
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene(sceneJson["name"]);

		// Iterate over game objects
		for (const auto& objectJson : sceneJson["gameObjects"])
		{
			std::shared_ptr<dae::GameObject> gameObject;

			// Set parent if exists
			if (objectJson.contains("parent"))
			{
				std::string parentName = objectJson["parent"];
				auto it = gameObjectMap.find(parentName);
				if (it != gameObjectMap.end())
				{
					gameObject = std::make_shared<dae::GameObject>(it->second.get());
				}
			}
			else
			{
				gameObject = std::make_shared<dae::GameObject>();
			}

			//
			if (objectJson.contains("name"))
			{
				std::string objectName = objectJson["name"];
				gameObjectMap[objectName] = gameObject; // Store the game object in the map
			}
			else
			{
				throw std::runtime_error("Game object name not found in JSON.");
			}

			// Set Local position
			if (objectJson.contains("position"))
			{
				auto position = objectJson["position"];
				gameObject->SetLocalPostion({ position[0], position[1], 0.f });
			}

			// Add Components
			if (objectJson.contains("components"))
			{
				for (const auto& componentJson : objectJson["components"])
				{
					//TextureComponent
					if (componentJson.contains("textureComponent"))
					{
						auto& textureComponent = gameObject->AddComponent<dae::TextureComponent>(
							texturePath,
							componentJson["textureComponent"]["x"],
							componentJson["textureComponent"]["y"],
							componentJson["textureComponent"]["width"],
							componentJson["textureComponent"]["height"],
							PACMAN_RENDERSCALE);
						if (componentJson["textureComponent"].contains("offset"))
						{
							auto offset = componentJson["textureComponent"]["offset"];
							textureComponent.SetOffset(offset[0] * PACMAN_RENDERSCALE, offset[1] * PACMAN_RENDERSCALE);
						}
					}

					//TextComponent
					else if (componentJson.contains("textComponent"))
					{
						gameObject->AddComponent<dae::TextComponent>(componentJson["textComponent"]["text"], Font);
					}

					//FpsComponent
					else if (componentJson.contains("fpsComponent"))
					{
						gameObject->AddComponent<dae::FpsComponent>();
					}

					//SpriteSheetAnimatorComponent
					else if (componentJson.contains("spriteSheetAnimator"))
					{
						auto& animator = gameObject->AddComponent<dae::SpriteSheetAnimator>();

						//Add All animations
						for (const auto& animationJson : componentJson["spriteSheetAnimator"]["animations"])
						{
							std::vector<dae::AnimationFrame> frames;
							for (const auto& frameJson : animationJson["frames"])
							{
								//                 x,            y,            width,        height,       duration
								frames.push_back({ frameJson[0], frameJson[1], frameJson[2], frameJson[3], frameJson[4] });
							}
							animator.AddAnimation(animationJson["name"], frames, animationJson["loop"]);
						}
					}

					//DieComponent
					else if (componentJson.contains("dieComponent"))
					{
						gameObject->AddComponent<dae::DieComponent>(std::make_unique<dae::Subject>());
					}

					//LivesUIComponent
					else if (componentJson.contains("livesUIComponent"))
					{
						gameObject->AddComponent<dae::LivesUIComponent>();
					}

					//PacmanGrid
					else if (componentJson.contains("pacmanGrid"))
					{
						auto& grid = gameObject->AddComponent<PacmanGrid>();
						grid.loadGrid(componentJson["pacmanGrid"]["gridFile"]);
					}

					//PacmanMovement
					else if (componentJson.contains("pacmanMovement"))
					{
						gameObject->AddComponent<PacmanMovement>(componentJson["pacmanMovement"]["speed"]);
						if (componentJson["pacmanMovement"].contains("grid"))
						{
							auto gridName = componentJson["pacmanMovement"]["grid"];
							auto it = gameObjectMap.find(gridName);
							if (it != gameObjectMap.end())
							{
								gameObject->GetComponent<PacmanMovement>()->SetGrid(it->second->GetComponent<PacmanGrid>());
							}
						}
					}


					// GhostMovement
					if (componentJson.contains("ghostMovement"))
					{
						float speed = componentJson["ghostMovement"]["speed"];
						auto& ghostMovement = gameObject->AddComponent<GhostMovement>(speed);

						if (componentJson["ghostMovement"].contains("grid"))
						{
							auto gridName = componentJson["ghostMovement"]["grid"];
							auto it = gameObjectMap.find(gridName);
							if (it != gameObjectMap.end())
							{
								ghostMovement.SetGrid(it->second->GetComponent<PacmanGrid>());
							}
						}
					}

					// GhostStateComponent
					else if (componentJson.contains("ghostStateComponent"))
					{
						// Create initial GhostState
						std::unique_ptr<GhostState> initialState = nullptr;

						if (componentJson["ghostStateComponent"].contains("initialState"))
						{
							std::string stateType = componentJson["ghostStateComponent"]["initialState"];
							if (stateType == "ChaseState")
							{
								initialState = std::make_unique<ChaseState>(gameObject.get());
							}
							// Add other states as needed
						}

						// Add GhostStateComponent
						gameObject->AddComponent<GhostStateComponent>(std::move(initialState));
					}

					//PickUpPelletsComponent
					else if (componentJson.contains("pickUpPelletsComponent"))
					{
						gameObject->AddComponent<dae::PickUpPelletsComponent>(std::make_unique<MsPacmanSubject>());
					}

					//ScoreUIcomponent
					else if (componentJson.contains("scoreUIComponent"))
					{
						gameObject->AddComponent<dae::ScoreUIcomponent>();
					}

					//RotatorComponent
					else if (componentJson.contains("rotatorComponent"))
					{
						gameObject->AddComponent<dae::RotatorComponent>(componentJson["rotatorComponent"]["speed"]);
					}
				}
			}

			//keyboardMovement
			if (objectJson.contains("keyboardMovement"))
			{
				dae::AddKeyboardMovement(gameObject.get());
			}

			//ControllerMovement
			if (objectJson.contains("controllerMovement"))
			{
				auto controllerIndex = objectJson["controllerMovement"]["controllerIndex"];
				dae::AddControllerMovement(controllerIndex, gameObject.get());
			}

			//Local position in grid
			if (objectJson.contains("gridPosition"))
			{
				auto gridName = objectJson["gridPosition"]["grid"];
				auto pos = objectJson["gridPosition"]["pos"];

				auto gridIt = gameObjectMap.find(gridName);
				if (gridIt != gameObjectMap.end())
				{
					auto grid = gridIt->second->GetComponent<PacmanGrid>();
					auto localPos = grid->GridToLocalPosition(pos[0], pos[1]);
					gameObject->SetLocalPostion({ localPos, 0.f });
				}
			}

			scene.Add(gameObject);
		}

		// Set up observer bindings
		if (sceneJson.contains("observerBindings"))
		{
			// Iterate over observer bindings
			for (const auto& binding : sceneJson["observerBindings"])
			{
				// Retrieve the subject GameObject and component
				std::string subjectObjectName = binding["subject"]["gameObject"];
				std::string subjectComponentType = binding["subject"]["component"];

				auto subjectObjectIt = gameObjectMap.find(subjectObjectName);
				if (subjectObjectIt == gameObjectMap.end())
				{
					assert(false && "Subject GameObject not found");
					continue;
				}
				auto subjectObject = subjectObjectIt->second;

				// Retrieve the observer GameObject and component
				std::string observerObjectName = binding["observer"]["gameObject"];
				std::string observerComponentType = binding["observer"]["component"];

				auto observerObjectIt = gameObjectMap.find(observerObjectName);
				if (observerObjectIt == gameObjectMap.end())
				{
					assert(false && "Observer GameObject not found");
					continue;
				}
				auto observerObject = observerObjectIt->second;

				// Now retrieve the specific components
				auto subjectComponent = GetComponentByName(subjectObject.get(), subjectComponentType);
				auto observerComponent = GetComponentByName(observerObject.get(), observerComponentType);

				if (!subjectComponent || !observerComponent)
				{
					assert(false && "Subject or Observer component not found");
					continue;
				}

				// Assuming subjectComponent has an AddObserver method
				AddObserverToSubject(subjectComponent, observerComponent);
			}
		}
	}
	//
	// Outside of the JSON file
	// 

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

	dae::SceneManager::GetInstance().SetActiveScene("Level1");
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}