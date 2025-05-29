#include "PacmanGrid.h"
#include <fstream>
#include "TextureComponent.h" // Required for SetSource
#include "Renderer.h" // Required for Renderer instance

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

void PacmanGrid::RegisterGhost(dae::GameObject* pGhost)
{
	m_pGhosts.emplace_back(pGhost);
}

void PacmanGrid::Update()
{
	if (!m_pMsPacman || m_pGhosts.empty())
	{
		return;
	}

	const auto pacmanGridPos = GetMsPacmanPos();

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
					}
				}
			}
		}
	}

	auto pos = m_pMsPacman->GetLocalPosition();
	auto gridPos = LocalToGridPosition(pos.x, pos.y);
	auto cellType = GetCellType(gridPos.first, gridPos.second);

	if (cellType == CellType::Pellet || cellType == CellType::BigPellet)
	{
		ConsumePellet(gridPos.first, gridPos.second);
	}

	setMsPacmanPos(gridPos.first, gridPos.second);
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
			std::cout << "All levels completed!" << std::endl;
			// Potentially notify game over or victory screen
		}
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
	Notify(MsPacmanEvent::START_LEVEL); // Notify observers that a new level has started
}