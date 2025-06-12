#include "PacmanGrid.h"
#include <fstream>
#include "TextureComponent.h" // Required for SetSource
#include "Renderer.h" // Required for Renderer instance
#include "LivesUIComponent.h"

void PacmanGrid::loadGrid(const std::string& filePath)
{
	std::ifstream file("../Data/" + filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open grid file: " << filePath << std::endl;
		return;
	}
	std::string line;
	int row = 0;
	m_TotalPelletsThisLevel = 0;
	m_PelletsEatenThisLevel = 0; 
	m_Grid.assign(m_Rows, std::vector<Cell>(m_Columns)); // Clear/resize grid
	while (std::getline(file, line) && row < m_Rows)
	{
		for (int column = 0; column < m_Columns && static_cast<size_t>(column) < line.size(); ++column)
		{
			char cell = line[column];
			m_Grid[row][column].type = static_cast<CellType>(cell - '0'); // Convert ASCII to integer value
			if (m_Grid[row][column].type == CellType::Pellet || m_Grid[row][column].type == CellType::BigPellet)
			{
				m_TotalPelletsThisLevel++;
			}
		}
		row++;
	}
	file.close();
	
}
void PacmanGrid::Render() const
{
	for (int row = 0; row < m_Rows; ++row)
	{
		for (int column = 0; column < m_Columns; ++column)
		{
			glm::vec2 pos = GridToWorldPosition(column, row);
			switch (m_Grid[row][column].type)
			{
			case CellType::Pellet:
				dae::Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, 8, 8, 8, 8, PACMAN_RENDERSCALE);
				break;
			case CellType::BigPellet:
				dae::Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, 8, 16, 8, 8, PACMAN_RENDERSCALE);
				break;
			}
		}
	}
}

void PacmanGrid::RegisterMsPacman(dae::GameObject* pMsPacman)
{
	m_pMsPacman = pMsPacman;
}

void PacmanGrid::RegisterPacman(dae::GameObject* pPacman)
{
	m_pPacman = pPacman;
}

void PacmanGrid::RegisterGhost(dae::GameObject* pGhost)
{
	m_pGhosts.emplace_back(pGhost);
}

void PacmanGrid::RegisterFruit(dae::GameObject* pFruit)
{
	m_pFruit = pFruit;
}

void PacmanGrid::RegisterLives(dae::GameObject* pLives)
{
	m_pLives = pLives;
}

void PacmanGrid::Update()
{
	UpdatePacman(m_pMsPacman);
	UpdatePacman(m_pPacman);
}


void PacmanGrid::UpdatePacman(dae::GameObject* pacman)
{
	if (!pacman || m_pGhosts.empty())
	{
		return;
	}

	glm::ivec2 pacmanGridPos;
	if (pacman == m_pMsPacman)
	{
		pacmanGridPos = GetMsPacmanPos();
		m_IsScoreForMsPacman = true;
	}
	else
	{
		pacmanGridPos = GetPacmanPos();
		m_IsScoreForMsPacman = false;
	}

	for (auto& pGhost : m_pGhosts)
	{
		if (!pGhost) continue;

		const auto ghostWorldPos = pGhost->GetWorldPosition();
		const auto ghostGridPosPair = WorldToGridPosition(ghostWorldPos.x, ghostWorldPos.y);
		const glm::ivec2 ghostGridPos = { ghostGridPosPair.first, ghostGridPosPair.second };

		if (ghostGridPos.x == pacmanGridPos.x && ghostGridPos.y == pacmanGridPos.y)
		{
			auto* ghostStateComp = pGhost->GetComponent<GhostStateComponent>();
			if (ghostStateComp)
			{
				GhostState* currentGhostState = ghostStateComp->GetState();
				if (currentGhostState)
				{
					GhostStateType stateType = currentGhostState->GetStateType();
					if (stateType == GhostStateType::FRIGHTENED)
					{
						Notify(MsPacmanEvent::EATEN_GHOST);
					}
					else if (stateType == GhostStateType::CHASE || stateType == GhostStateType::START)
					{
						Notify(MsPacmanEvent::DIE);
						int livesLeft = m_pLives->GetComponent<LivesUIComponent>()->GetLives();

						if (livesLeft < 0)
						{
							Notify(MsPacmanEvent::GAME_OVER);
							std::cout << "Game Over! No lives left." << std::endl;
						}
					}
				}
			}
		}
	}

	if (m_pFruit && !m_FruitEaten)
	{
		const auto fruitWorldPos = m_pFruit->GetWorldPosition();
		const auto fruitGridPosPair = WorldToGridPosition(fruitWorldPos.x, fruitWorldPos.y);
		const glm::ivec2 fruitGridPos = { fruitGridPosPair.first, fruitGridPosPair.second };
		if (fruitGridPos.x == pacmanGridPos.x && fruitGridPos.y == pacmanGridPos.y)
		{
			m_FruitEaten = true;
			Notify(static_cast<MsPacmanEvent>((int)MsPacmanEvent::EATEN_CHERRY + m_CurrentLevelIndex));
		}
	}

	auto pos = pacman->GetLocalPosition();
	auto gridPos = LocalToGridPosition(pos.x, pos.y);
	auto cellType = GetCellType(gridPos.first, gridPos.second);

	if (cellType == CellType::Pellet || cellType == CellType::BigPellet)
	{
		ConsumePellet(gridPos.first, gridPos.second);
	}

	SetPacmanPos(pacman, { gridPos.first,gridPos.second });
}

void PacmanGrid::ConsumePellet(int column, int row)
{
	if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
	{
		std::cout << "ConsumePellet: Out of bounds" << std::endl;
		return; // Out of bounds
	}
	if (m_Grid[row][column].type == CellType::Pellet)
	{
		Notify(MsPacmanEvent::EATEN_SMALL_PELLET);
		m_Grid[row][column].type = CellType::Empty;
		m_PelletsEatenThisLevel++;
		std::cout << "Pellet consumed at (" << column << ", " << row << ")" << std::endl;
	}
	else if (m_Grid[row][column].type == CellType::BigPellet)
	{
		Notify(MsPacmanEvent::EATEN_BIG_PELLET);
		m_Grid[row][column].type = CellType::Empty;
		m_PelletsEatenThisLevel++;
		std::cout << "Big pellet consumed at (" << column << ", " << row << ")" << std::endl;
	}
	else
	{
		std::cout << "No pellet at (" << column << ", " << row << ")" << std::endl;
		return; // Return if not a pellet to avoid checking completion unnecessarily
	}

	if (m_TotalPelletsThisLevel > 0 && m_PelletsEatenThisLevel >= m_TotalPelletsThisLevel) // Ensure total pellets is > 0 to avoid issues with empty levels
	{
		Notify(MsPacmanEvent::LEVEL_COMPLETE);
		std::cout << "All pellets consumed for this level!" << std::endl;
		if (m_CurrentLevelIndex + 1 < static_cast<int>(m_Levels.size()))
		{
			LoadLevel(m_CurrentLevelIndex + 1);
		}
		else
		{
			Notify(MsPacmanEvent::GAME_OVER);
			std::cout << "All levels completed!" << std::endl;
			// Potentially notify game over or victory screen
		}
	}

	else if (m_PelletsEatenThisLevel == 64)
	{
		Notify(MsPacmanEvent::SPAWN_FRUIT);
	}
}

void PacmanGrid::AddLevelData(const LevelData& levelData)
{
	m_Levels.push_back(levelData);
}

void PacmanGrid::LoadLevel(int levelIndex)
{
	if (levelIndex < 0 || levelIndex >= static_cast<int>(m_Levels.size()))
	{
		std::cerr << "LoadLevel: Invalid level index " << levelIndex << std::endl;
		return;
	}

	m_CurrentLevelIndex = levelIndex;
	const auto& currentLevelData = m_Levels[m_CurrentLevelIndex];

	loadGrid(currentLevelData.gridFilePath);

	if (GetOwner())
	{
		auto* textureComp = GetOwner()->GetComponent<dae::TextureComponent>();
		if (textureComp)
		{
			textureComp->SetSource(currentLevelData.mazeTextureSourceX,
				currentLevelData.mazeTextureSourceY,
				currentLevelData.mazeTextureSourceWidth,
				currentLevelData.mazeTextureSourceHeight);
		}
		else
		{
			std::cerr << "PacmanGrid::LoadLevel - Owner GameObject does not have a TextureComponent." << std::endl;
		}
	}
	else
	{
		std::cerr << "PacmanGrid::LoadLevel - PacmanGrid does not have an owner GameObject." << std::endl;
	}

	std::cout << "Loaded level " << m_CurrentLevelIndex << " with grid: " << currentLevelData.gridFilePath << std::endl;

	m_FruitEaten = false;
	Notify(MsPacmanEvent::START_LEVEL); // Notify observers that a new level has started
}