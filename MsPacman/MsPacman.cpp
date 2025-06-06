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
#include "PickUpPelletsComponent.h"
#include "Scene.h"
#include "MsPacmanSubject.h"
#include "ServiceLocator.h"
#include "PacmanMovement.h"
#include "GhostMovement.h"
#include "GhostStateComponent.h"
#include "ChaseState.h"
#include "FrightenedState.h"
#include "EatenState.h"
#include "StartState.h"
#include "SpriteSheetAnimatorComponent.h"
#include "PacmanGrid.h"
#include "SoundManager.h"
#include "SoundServiceSDL.h"
#include "MsPacmanDieComponent.h"
#include "ScorePopUp.h"
#include "FruitBehavior.h"
#include "ButtonComponent.h"
#include "ButtonManagerComponent.h"
#include "MsPacmanButtonCommands.h"
#include "UIBinding.h" 

#include  "CustomPacmanDefines.h"

// Helper struct for deferred grid registration
struct PendingGridRegistration
{
	PacmanGrid* gridInstance{ nullptr };
	std::string msPacmanObjectName;
	std::string fruitObjectName;
	std::vector<std::string> ghostObjectNames;
};

struct PendingButtonRegistration
{
    dae::ButtonManagerComponent* managerInstance{ nullptr };
    std::vector<std::string> buttonObjectNames;
};

using ComponentGetter = std::function<dae::Component* (dae::GameObject*)>;

std::unordered_map<std::string, ComponentGetter> componentGetters = {
	{ "pacmanGrid", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<PacmanGrid>(); } },
	{ "scoreUIComponent", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<dae::ScoreUIcomponent>(); } },
	{ "livesUIComponent", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<LivesUIComponent>(); } },
	{ "dieComponent", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<MsPacmanDieComponent>(); } },
	{ "pickUpPelletsComponent", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<dae::PickUpPelletsComponent>(); } },
	{ "ghostStateComponent", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<GhostStateComponent>(); } },
	{ "ghostMovement", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<GhostMovement>(); } },
	{ "scorePopUp", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<ScorePopUp>(); } },
	{ "fruitBehavior", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<FruitBehavior>(); } },
	{ "buttonComponent", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<dae::ButtonComponent>(); } },
	{ "buttonManagerComponent", [](dae::GameObject* obj) -> dae::Component* { return obj->GetComponent<dae::ButtonManagerComponent>(); } },
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

std::shared_ptr<dae::Command> CreateCommand(const std::string& commandType)
{
    // Add commands as needed based on your game's requirements
    if (commandType == "startSoloGame") {
        return std::make_shared<StartSoloLevelCommand>();
    }
	else if (commandType == "startCoopGame") {
		return std::make_shared<StartCoopLevelCommand>();
	}
	else if (commandType == "startVersusGame") {
		return std::make_shared<StartVersusLevelCommand>();
	}
    else if (commandType == "quitGame") {
        return std::make_shared<QuitGameCommand>();
    }

    return nullptr;
}

void AddObserverToSubject(dae::Component* subjectComponent, dae::Component* observerComponent)
{
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

void loadGameJSON(const std::string& path)
{
	auto Font = dae::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 24);
	std::string texturePath = "spritesheet.png";

	std::ifstream file(path);
	nlohmann::json sceneData;
	file >> sceneData;

	std::unordered_map<std::string, std::shared_ptr<dae::GameObject>> gameObjectMap;
	std::vector<PendingGridRegistration> allPendingGridRegistrations; // To store registration tasks
	std::vector<PendingButtonRegistration> allPendingButtonRegistrations; // To store button registration tasks

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

					//LivesUIComponent
					else if (componentJson.contains("livesUIComponent"))
					{
						gameObject->AddComponent<LivesUIComponent>();
					}

					//MsPacmanDieComponent
					else if (componentJson.contains("dieComponent"))
					{
						gameObject->AddComponent<MsPacmanDieComponent>();
					}

					//PacmanGrid
					else if (componentJson.contains("pacmanGrid"))
					{
						auto& gridComponentJson = componentJson["pacmanGrid"];
						auto& grid = gameObject->AddComponent<PacmanGrid>();

						if (gridComponentJson.contains("levels") && gridComponentJson["levels"].is_array())
						{
							for (const auto& levelJson : gridComponentJson["levels"])
							{
								LevelData levelData;
								levelData.gridFilePath = levelJson["gridFile"].get<std::string>();
								levelData.mazeTextureSourceX = levelJson["textureSourceX"].get<int>();
								levelData.mazeTextureSourceY = levelJson["textureSourceY"].get<int>();
								levelData.mazeTextureSourceWidth = levelJson["textureSourceWidth"].get<int>();
								levelData.mazeTextureSourceHeight = levelJson["textureSourceHeight"].get<int>();
								grid.AddLevelData(levelData);
							}
						}
						else
						{
							throw std::runtime_error("PacmanGrid component in JSON must contain a 'levels' array.");
						}

						int initialLevel = 0;
						if (gridComponentJson.contains("initialLevelIndex"))
						{
							initialLevel = gridComponentJson["initialLevelIndex"].get<int>();
						}
						grid.LoadLevel(initialLevel); // Load the initial level

						PendingGridRegistration pendingReg;
						pendingReg.gridInstance = &grid;

						if (gridComponentJson.contains("registerMsPacman"))
						{
							pendingReg.msPacmanObjectName = gridComponentJson["registerMsPacman"];
						}

						if (gridComponentJson.contains("registerFruit"))
						{
							pendingReg.fruitObjectName = gridComponentJson["registerFruit"];
						}

						if (gridComponentJson.contains("registerGhosts"))
						{
							for (const auto& ghostNameJson : gridComponentJson["registerGhosts"])
							{
								pendingReg.ghostObjectNames.push_back(ghostNameJson.get<std::string>());
							}
						}
						allPendingGridRegistrations.push_back(pendingReg);
					}

					//PacmanMovement
					else if (componentJson.contains("pacmanMovement"))
					{
						gameObject->AddComponent<PacmanMovement>(componentJson["pacmanMovement"]["speed"]);
						if (componentJson["pacmanMovement"].contains("grid"))
						{
							auto gridName = componentJson["pacmanMovement"]["grid"];
							auto it = gameObjectMap.find(gridName.get<std::string>()); // Ensure gridName is used as string
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
							auto it = gameObjectMap.find(gridName.get<std::string>()); // Ensure gridName is used as string
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
							else if (stateType == "FrightenedState")
							{
								initialState = std::make_unique<FrightenedState>(gameObject.get());
							}
							else if (stateType == "StartState")
							{
								initialState = std::make_unique<StartState>(gameObject.get());
							}
							else if (stateType == "EatenState")
							{
								initialState = std::make_unique<EatenState>(gameObject.get());
							}
							else
							{
								assert(false && "Unknown GhostState type");
							}
						}

						GhostType type{ GhostType::Blinky }; // Default value
						glm::ivec2 startPosition{ 0, 0 }; // Default value
						if (componentJson["ghostStateComponent"].contains("ghostType"))
						{
							std::string ghostType = componentJson["ghostStateComponent"]["ghostType"];
							if (ghostType == "Blinky")
							{
								type = GhostType::Blinky;
								startPosition = { 13, 11 };
							}
							else if (ghostType == "Pinky")
							{
								type = GhostType::Pinky;
								startPosition = { 14, 14 };

							}
							else if (ghostType == "Inky")
							{
								type = GhostType::Inky;
								startPosition = { 12, 14 };

							}
							else if (ghostType == "Clyde")
							{
								type = GhostType::Clyde;
								startPosition = { 16, 14 };
							}
						}
						// Add GhostStateComponent
						gameObject->AddComponent<GhostStateComponent>(std::move(initialState), type, startPosition);
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

					//ScorePopUp
					else if (componentJson.contains("scorePopUp"))
					{
						auto& scorePopUp = gameObject->AddComponent<ScorePopUp>();
						// If the parent is the grid, set the grid reference
						if (gameObject->GetParent() && gameObject->GetParent()->GetComponent<PacmanGrid>())
						{
							scorePopUp.SetGrid(gameObject->GetParent()->GetComponent<PacmanGrid>());
						}
					}

					//FruitBehavior
					else if (componentJson.contains("fruitBehavior"))
					{
						gameObject->AddComponent<FruitBehavior>();
					}

					//RotatorComponent
					else if (componentJson.contains("rotatorComponent"))
					{
						gameObject->AddComponent<dae::RotatorComponent>(componentJson["rotatorComponent"]["speed"]);
					}

					//ButtonComponent
					else if (componentJson.contains("buttonComponent"))
					{
						float width = componentJson["buttonComponent"]["width"];
						float height = componentJson["buttonComponent"]["height"];
						auto& buttonComp = gameObject->AddComponent<dae::ButtonComponent>(width, height);
						
						// If there's a command specified, set it
						if (componentJson["buttonComponent"].contains("command"))
						{
							std::string commandType = componentJson["buttonComponent"]["command"];
							auto command = CreateCommand(commandType);
							if (command)
							{
								buttonComp.SetCommand(command);
							}
						}
					}

					//ButtonManagerComponent
					else if (componentJson.contains("buttonManagerComponent"))
					{
						auto& buttonManager = gameObject->AddComponent<dae::ButtonManagerComponent>();
						
						// Store button references for deferred processing
						if (componentJson["buttonManagerComponent"].contains("buttons"))
						{
							PendingButtonRegistration pendingReg;
							pendingReg.managerInstance = &buttonManager;
							
							for (const auto& buttonNameJson : componentJson["buttonManagerComponent"]["buttons"])
							{
								pendingReg.buttonObjectNames.push_back(buttonNameJson.get<std::string>());
							}
							
							allPendingButtonRegistrations.push_back(pendingReg);
						}
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

				auto gridIt = gameObjectMap.find(gridName.get<std::string>()); // Ensure gridName is used as string
				if (gridIt != gameObjectMap.end())
				{
					auto grid = gridIt->second->GetComponent<PacmanGrid>();
					auto localPos = grid->GridToLocalPosition(pos[0], pos[1]);
					gameObject->SetLocalPostion({ localPos, 0.f });
				}
			}

			//UI Keyboard Binding
			if (objectJson.contains("uiKeyboardBinding"))
			{
				if (gameObject->HasComponent<dae::ButtonManagerComponent>())
				{
					AddUIKeyboardBinding(gameObject.get());
				}
				else
				{
					std::cerr << "Warning: uiKeyboardBinding specified for object without ButtonManagerComponent" << std::endl;
				}
			}

			//UI controller binding
			if (objectJson.contains("uiControllerBinding"))
			{
				if (gameObject->HasComponent<dae::ButtonManagerComponent>())
				{
					int controllerIndex = objectJson["uiControllerBinding"]["controllerIndex"];
					AddUIControllerBinding(controllerIndex, gameObject.get());
				}
				else
				{
					std::cerr << "Warning: uiKeyboardBinding specified for object without ButtonManagerComponent" << std::endl;
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

	// Process all pending grid registrations after all game objects from all scenes are created
	for (const auto& regInfo : allPendingGridRegistrations)
	{
		if (!regInfo.gridInstance) continue;

		if (!regInfo.msPacmanObjectName.empty())
		{
			auto it = gameObjectMap.find(regInfo.msPacmanObjectName);
			if (it != gameObjectMap.end())
			{
				regInfo.gridInstance->RegisterMsPacman(it->second.get());
			}
			else
			{
				std::cerr << "Error: Could not find MsPacman GameObject with name '" << regInfo.msPacmanObjectName << "' for grid registration post-load." << std::endl;
			}
		}

		if (!regInfo.fruitObjectName.empty())
		{
			auto it = gameObjectMap.find(regInfo.fruitObjectName);
			if (it != gameObjectMap.end())
			{
				regInfo.gridInstance->RegisterFruit(it->second.get());
			}
			else
			{
				std::cerr << "Error: Could not find Fruit GameObject with name '" << regInfo.fruitObjectName << "' for grid registration post-load." << std::endl;
			}
		}

		for (const auto& ghostName : regInfo.ghostObjectNames)
		{
			auto it = gameObjectMap.find(ghostName);
			if (it != gameObjectMap.end())
			{
				regInfo.gridInstance->RegisterGhost(it->second.get());
			}
			else
			{
				std::cerr << "Error: Could not find Ghost GameObject with name '" << ghostName << "' for grid registration post-load." << std::endl;
			}
		}
	}

	// Process all pending button registrations after all game objects from all scenes are created
	for (const auto& regInfo : allPendingButtonRegistrations)
	{
		if (!regInfo.managerInstance) continue;

		for (const auto& buttonName : regInfo.buttonObjectNames)
		{
			auto it = gameObjectMap.find(buttonName);
			if (it != gameObjectMap.end())
			{
				regInfo.managerInstance->AddButton(it->second.get());
			}
			else
			{
				std::cerr << "Error: Could not find Button GameObject with name '" << buttonName << "' for button manager registration post-load." << std::endl;
			}
		}
	}
}

void load()
{
	loadGameJSON("../Data/MainMenu.json");
	loadGameJSON("../Data/Solo.json");
	

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