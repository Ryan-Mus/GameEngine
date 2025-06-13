#pragma once
#include "GameLoader.h"

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
#include "MainMenuButtonCommands.h"
#include "UIBinding.h" 
#include "HighScoreManager.h"
#include "HighscoreCommands.h"
#include "HighscoreBinding.h"
#include "FruitUIComponent.h"
#include "FKeyBindings.h"
#include "PlayerGhostBindings.h"

#include  "CustomPacmanDefines.h"

struct CommandParameters
{
	std::string commandType;
	std::string gameObjectName;
	dae::GameObject* gameObject{ nullptr };
};

struct PendingGridRegistration
{
	PacmanGrid* gridInstance{ nullptr };
	std::string msPacmanObjectName;
	std::string pacmanObjectName;
	std::string fruitObjectName;
	std::string livesObjectName;
	std::vector<std::string> ghostObjectNames;
};

struct PendingFruitRegistration
{
	FruitUIComponent* fruitUIComponent{ nullptr };
	std::string cherryIconObject;
	std::string strawberryIconObject;
	std::string orangeIconObject;
};

struct PendingButtonRegistration
{
	dae::ButtonManagerComponent* managerInstance{ nullptr };
	std::vector<std::string> buttonObjectNames;
};

struct PendingHighscoreRegistration
{
	HighScoreManager* highscoreManager{ nullptr };
	std::vector<std::string>nameDisplaysObjectName;
	std::string lastScoreDisplayObjectName;
	std::vector<std::string>highscoreDisplaysObjectName;
};

struct PendingHighscoreBindingRegistration
{
	dae::GameObject* pHighscoreManagerObject;
	std::string buttonManagerName;
	std::string sceneName;
};

struct PendingCommandRegistration
{
	dae::ButtonComponent* buttonComponent{ nullptr };
	CommandParameters commandParameters;
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
	{ "fruitUIComponent", [](dae::GameObject* obj) -> dae::Component* {return obj->GetComponent<FruitUIComponent>(); } },
	{ "pacmanMovement", [](dae::GameObject* obj) -> dae::Component* {return obj->GetComponent<PacmanMovement>(); } }
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



std::shared_ptr<dae::Command> CreateCommand(const CommandParameters& command)
{
	// Add commands as needed based on your game's requirements
	if (command.commandType == "startSoloGame") {
		return std::make_shared<StartSoloLevelCommand>();
	}
	else if (command.commandType == "startCoopGame") {
		return std::make_shared<StartCoopLevelCommand>();
	}
	else if (command.commandType == "startVersusGame") {
		return std::make_shared<StartVersusLevelCommand>();
	}
	else if (command.commandType == "quitGame") {
		return std::make_shared<QuitGameCommand>();
	}
	else if (command.commandType == "loadHighscore") {
		return std::make_shared<LoadHighscoreCommand>(command.gameObject);
	}
	else if (command.commandType == "saveHighscore") {
		return std::make_shared<SaveHighscoreCommand>(command.gameObject);
	}
	else if (command.commandType == "openMainMenu")
	{
		return std::make_shared<OpenMainMenuCommand>();
	}
	else if (command.commandType == "openHighscoreMenu")
	{
		return std::make_shared<OpenHighscoreMenuCommand>();
	}
	else if (command.commandType == "null")
	{
		return std::make_shared<NullCommand>();
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

void GameLoader::loadGameJSON(const std::string& path)
{
	auto Font = dae::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 24);
	std::string texturePath = "spritesheet.png";

	std::ifstream file(path);
	nlohmann::json sceneData;
	file >> sceneData;

	std::unordered_map<std::string, dae::GameObject*> gameObjectMap;
	std::vector<PendingGridRegistration> allPendingGridRegistrations; // To store registration tasks
	std::vector<PendingButtonRegistration> allPendingButtonRegistrations; // To store button registration tasks
	std::vector<PendingHighscoreRegistration> allPendingHighscoreDisplayRegistrations; // To store highscore display registrations
	std::vector<PendingCommandRegistration> allPendingCommandRegistrations; // To store command registrations
	std::vector<PendingHighscoreBindingRegistration> allPendingHighscoreBindingRegistrations;
	std::vector<PendingFruitRegistration> allPendingFruitRegistrations;

	std::vector<dae::GameObject*> HighScoreManagerObjects;

	// Iterate over scenes
	for (const auto& sceneJson : sceneData["scenes"])
	{
		auto& scene = dae::SceneManager::GetInstance().CreateScene(sceneJson["name"]);

		// Iterate over game objects
		for (const auto& objectJson : sceneJson["gameObjects"])
		{
			std::unique_ptr<dae::GameObject> gameObject;

			// Set parent if exists
			if (objectJson.contains("parent"))
			{
				std::string parentName = objectJson["parent"];
				auto it = gameObjectMap.find(parentName);
				if (it != gameObjectMap.end())
				{
					gameObject = std::make_unique<dae::GameObject>(it->second);
				}
			}
			else
			{
				gameObject = std::make_unique<dae::GameObject>();
			}

			//
			if (objectJson.contains("name"))
			{
				std::string objectName = objectJson["name"];
				gameObjectMap[objectName] = gameObject.get(); // Store the game object in the map
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

						if (gridComponentJson.contains("registerPacman"))
						{
							pendingReg.pacmanObjectName = gridComponentJson["registerPacman"];
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

						if (gridComponentJson.contains("registerLives"))
						{
							pendingReg.livesObjectName = gridComponentJson["registerLives"];
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
							else if (ghostType == "PlayerGhost")
							{
								type = GhostType::PlayerGhost;
								startPosition = { 13, 11 };
							}
						}

						// Add GhostStateComponent
						auto& comp = gameObject->AddComponent<GhostStateComponent>(std::move(initialState), type, startPosition);

						if (componentJson["ghostStateComponent"].contains("playerMovement"))
						{
							//AddKeyboardGhostMovement(&comp,sceneJson["name"]);
							if (componentJson["ghostStateComponent"]["playerMovement"].contains("controllerIndex"))
							{
								int controllerIndex = componentJson["ghostStateComponent"]["playerMovement"]["controllerIndex"];
								AddControllerGhostMovement(controllerIndex, &comp, sceneJson["name"]);
							}
						}

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
							PendingCommandRegistration pendingReg;
							pendingReg.buttonComponent = &buttonComp;
							CommandParameters commandPara{};
							commandPara.commandType = componentJson["buttonComponent"]["command"]["type"];
							if (componentJson["buttonComponent"]["command"].contains("gameObject"))
							{
								commandPara.gameObjectName = componentJson["buttonComponent"]["command"]["gameObject"];
							}

							pendingReg.commandParameters = commandPara;
							allPendingCommandRegistrations.push_back(pendingReg);
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

					//HighScoreManager
					else if (componentJson.contains("highScoreManager"))
					{
						auto& highscoreManager = gameObject->AddComponent<HighScoreManager>();
						HighScoreManagerObjects.push_back(gameObject.get()); // Store the HighScoreManager object for later processing
						// Store highscore display references for deferred processing
						if (componentJson["highScoreManager"].contains("highscoreDisplays"))
						{
							PendingHighscoreRegistration pendingReg;
							pendingReg.highscoreManager = &highscoreManager;
							for (const auto& displayNameJson : componentJson["highScoreManager"]["highscoreDisplays"])
							{
								pendingReg.highscoreDisplaysObjectName.push_back(displayNameJson.get<std::string>());
							}
							for (const auto& nameDisplayNameJson : componentJson["highScoreManager"]["nameDisplays"])
							{
								pendingReg.nameDisplaysObjectName.push_back(nameDisplayNameJson.get<std::string>());
							}

							if (componentJson["highScoreManager"].contains("lastScoreDisplay"))
							{
								pendingReg.lastScoreDisplayObjectName = componentJson["highScoreManager"]["lastScoreDisplay"].get<std::string>();
							}
							allPendingHighscoreDisplayRegistrations.push_back(pendingReg);

							PendingHighscoreBindingRegistration pendingBindingReg;
							pendingBindingReg.pHighscoreManagerObject = gameObject.get();
							pendingBindingReg.sceneName = sceneJson["name"].get<std::string>();
							if (componentJson["highScoreManager"].contains("highscoreBinding"))
							{
								pendingBindingReg.buttonManagerName = componentJson["highScoreManager"]["highscoreBinding"].get<std::string>();
								allPendingHighscoreBindingRegistrations.push_back(pendingBindingReg);
							}
						}
					}

					//FruitUIComponent
					else if (componentJson.contains("fruitUIComponent"))
					{
						auto& fruitUIComponent = gameObject->AddComponent<FruitUIComponent>();
						PendingFruitRegistration pendingReg;
						pendingReg.fruitUIComponent = &fruitUIComponent;
						if (componentJson["fruitUIComponent"].contains("cherryIcon"))
						{
							pendingReg.cherryIconObject = componentJson["fruitUIComponent"]["cherryIcon"];
						}
						if (componentJson["fruitUIComponent"].contains("strawberryIcon"))
						{
							pendingReg.strawberryIconObject = componentJson["fruitUIComponent"]["strawberryIcon"];
						}
						if (componentJson["fruitUIComponent"].contains("orangeIcon"))
						{
							pendingReg.orangeIconObject = componentJson["fruitUIComponent"]["orangeIcon"];
						}

						allPendingFruitRegistrations.push_back(pendingReg);
					}
				}
			}
			//keyboardMovement
			if (objectJson.contains("keyboardMovement"))
			{
				dae::AddKeyboardMovement(gameObject.get(), sceneJson["name"]);
			}

			//ControllerMovement
			if (objectJson.contains("controllerMovement"))
			{
				auto controllerIndex = objectJson["controllerMovement"]["controllerIndex"];
				dae::AddControllerMovement(controllerIndex, gameObject.get(), sceneJson["name"]);
			}

			//SkipLevel
			if (objectJson.contains("skipLevel"))
			{
				AddSkipLevelBinding(gameObject.get(), sceneJson["name"]);
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
					AddUIKeyboardBinding(gameObject.get(), sceneJson["name"]);
				}
				else
				{
					std::cout << "Warning: uiKeyboardBinding specified for object without ButtonManagerComponent" << std::endl;
				}
			}

			//UI controller binding
			if (objectJson.contains("uiControllerBinding"))
			{
				if (gameObject->HasComponent<dae::ButtonManagerComponent>())
				{
					int controllerIndex = objectJson["uiControllerBinding"]["controllerIndex"];
					AddUIControllerBinding(controllerIndex, gameObject.get(), sceneJson["name"]);
				}
				else
				{
					std::cout << "Warning: uiKeyboardBinding specified for object without ButtonManagerComponent" << std::endl;
				}
			}


			scene.Add(std::move(gameObject));
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
				auto subjectComponent = GetComponentByName(subjectObject, subjectComponentType);
				auto observerComponent = GetComponentByName(observerObject, observerComponentType);

				if (!subjectComponent || !observerComponent)
				{
					assert(false && "Subject or Observer component not found");
					continue;
				}

				// Assuming subjectComponent has an AddObserver method
				AddObserverToSubject(subjectComponent, observerComponent);
			}
		}

		if (sceneJson.contains("soundBindings"))
		{
			AddSoundBindings();
		}
	}

	// Process all pending grid registrations after all game objects from all scenes are created
	for (const auto& regInfo : allPendingGridRegistrations)
	{
		if (!regInfo.gridInstance) continue;

		regInfo.gridInstance->Notify(MsPacmanEvent::START_LEVEL);

		if (!regInfo.msPacmanObjectName.empty())
		{
			auto it = gameObjectMap.find(regInfo.msPacmanObjectName);
			if (it != gameObjectMap.end())
			{
				regInfo.gridInstance->RegisterMsPacman(it->second);
			}
			else
			{
				std::cout << "Error: Could not find MsPacman GameObject with name '" << regInfo.msPacmanObjectName << "' for grid registration post-load." << std::endl;
			}
		}

		if (!regInfo.pacmanObjectName.empty())
		{
			auto it = gameObjectMap.find(regInfo.pacmanObjectName);
			if (it != gameObjectMap.end())
			{
				regInfo.gridInstance->RegisterPacman(it->second);
			}
			else
			{
				std::cout << "Error: Could not find Pacman GameObject with name '" << regInfo.pacmanObjectName << "' for grid registration post-load." << std::endl;
			}
		}

		if (!regInfo.fruitObjectName.empty())
		{
			auto it = gameObjectMap.find(regInfo.fruitObjectName);
			if (it != gameObjectMap.end())
			{
				regInfo.gridInstance->RegisterFruit(it->second);
			}
			else
			{
				std::cout << "Error: Could not find Fruit GameObject with name '" << regInfo.fruitObjectName << "' for grid registration post-load." << std::endl;
			}
		}

		for (const auto& ghostName : regInfo.ghostObjectNames)
		{
			auto it = gameObjectMap.find(ghostName);
			if (it != gameObjectMap.end())
			{
				regInfo.gridInstance->RegisterGhost(it->second);
			}
			else
			{
				std::cout << "Error: Could not find Ghost GameObject with name '" << ghostName << "' for grid registration post-load." << std::endl;
			}
		}
		if (!regInfo.livesObjectName.empty())
		{
			auto it = gameObjectMap.find(regInfo.livesObjectName);
			if (it != gameObjectMap.end())
			{
				regInfo.gridInstance->RegisterLives(it->second);
			}
			else
			{
				std::cout << "Error: Could not find Lives UI GameObject with name '" << regInfo.livesObjectName << "' for grid registration post-load." << std::endl;
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
				regInfo.managerInstance->AddButton(it->second);
			}
			else
			{
				std::cout << "Error: Could not find Button GameObject with name '" << buttonName << "' for button manager registration post-load." << std::endl;
			}
		}
	}

	// Process highscore display registrations
	for (const auto& regInfo : allPendingHighscoreDisplayRegistrations)
	{
		if (!regInfo.highscoreManager) continue;
		for (int i{}; i < static_cast<int>(regInfo.highscoreDisplaysObjectName.size()); ++i)
		{
			auto it = gameObjectMap.find(regInfo.highscoreDisplaysObjectName[i]);
			if (it != gameObjectMap.end())
			{
				regInfo.highscoreManager->AddHighscoreDisplay(it->second, i);
			}
			else
			{
				std::cout << "Error: Could not find Highscore Display GameObject with name '" << regInfo.highscoreDisplaysObjectName[i] << "' for highscore manager registration post-load." << std::endl;
			}
		}

		for (int i{}; i < static_cast<int>(regInfo.nameDisplaysObjectName.size()); ++i)
		{
			auto it = gameObjectMap.find(regInfo.nameDisplaysObjectName[i]);
			if (it != gameObjectMap.end())
			{
				regInfo.highscoreManager->AddNameDisplay(it->second, i);
			}
			else
			{
				std::cout << "Error: Could not find name Display GameObject with name '" << regInfo.nameDisplaysObjectName[i] << "' for highscore manager registration post-load." << std::endl;
			}
		}

		if (!regInfo.lastScoreDisplayObjectName.empty())
		{
			auto it = gameObjectMap.find(regInfo.lastScoreDisplayObjectName);
			if (it != gameObjectMap.end())
			{
				regInfo.highscoreManager->SetLastHighScoreDisplay(it->second);
			}
		}
	}

	for (auto& regInfo : allPendingCommandRegistrations)
	{
		if (!regInfo.buttonComponent) continue;

		if (!regInfo.commandParameters.gameObjectName.empty())
		{
			auto it = gameObjectMap.find(regInfo.commandParameters.gameObjectName);
			if (it != gameObjectMap.end())
			{
				regInfo.commandParameters.gameObject = it->second;
			}
			else
			{
				std::cout << "Error: Could not find GameObject with name '" << regInfo.commandParameters.gameObjectName << "' for command registration post-load." << std::endl;
				continue; // Skip this command if the GameObject is not found
			}
		}

		auto command = CreateCommand(regInfo.commandParameters);
		if (command)
		{
			regInfo.buttonComponent->SetCommand(command);
		}
		else
		{
			std::cout << "Error: Could not create command for button registration post-load." << std::endl;
		}
	}
	for (auto& regInfo : allPendingHighscoreBindingRegistrations)
	{
		if (!regInfo.buttonManagerName.empty())
		{
			auto it = gameObjectMap.find(regInfo.buttonManagerName);
			if (it != gameObjectMap.end())
			{
				AddHighscoreKeyboardBinding(it->second, regInfo.pHighscoreManagerObject, regInfo.sceneName);
				AddHighScoreControllerBinding(0, it->second, regInfo.pHighscoreManagerObject, regInfo.sceneName);
			}
		}
	}

	for (auto& highscoreManagerObject : HighScoreManagerObjects)
	{
		auto highscoreManager = highscoreManagerObject->GetComponent<HighScoreManager>();
		if (highscoreManager)
		{
			highscoreManager->LoadHighScores();
		}
		else
		{
			std::cout << "Error: HighScoreManager component not found on GameObject." << std::endl;
		}
	}

	for (auto& regInfo : allPendingFruitRegistrations)
	{
		if (regInfo.fruitUIComponent)
		{
			auto it = gameObjectMap.find(regInfo.cherryIconObject);
			if (it != gameObjectMap.end())
			{
				regInfo.fruitUIComponent->RegisterFruit(it->second);
			}
			it = gameObjectMap.find(regInfo.strawberryIconObject);
			if (it != gameObjectMap.end())
			{
				regInfo.fruitUIComponent->RegisterFruit(it->second);
			}
			it = gameObjectMap.find(regInfo.orangeIconObject);
			if (it != gameObjectMap.end())
			{
				regInfo.fruitUIComponent->RegisterFruit(it->second);
			}
		}
	}
}