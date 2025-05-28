#include "PacmanGrid.h"
#include <fstream>


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
	while (std::getline(file, line) && row < m_Rows)
	{
		for (int column = 0; column < m_Columns && static_cast<size_t>(column) < line.size(); ++column)
		{
			char cell = line[column];
			m_Grid[row][column].type = static_cast<CellType>(cell - '0'); // Convert ASCII to integer value
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
				dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, 8, 8, 8, 8, PACMAN_RENDERSCALE);
				break;
			case CellType::BigPellet:
				dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, 8, 16, 8, 8, PACMAN_RENDERSCALE);
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

	// The existing setMsPacmanPos seems to track pacman's position based on movement completion.
	// For direct collision, we should use its current world position converted to grid cell.
	// However, PacmanGrid already has m_MsPacmanPos which is updated by setMsPacmanPos.
	// Let's assume m_MsPacmanPos is the authoritative grid position for MsPacman.
	// If not, this logic might need adjustment based on how MsPacman's position is truly managed.
	const auto pacmanGridPos = GetMsPacmanPos();

	for (auto& pGhost : m_pGhosts)
	{
		if (!pGhost) continue;

		const auto ghostWorldPos = pGhost->GetWorldPosition();
		const auto ghostGridPosPair = WorldToGridPosition(ghostWorldPos.x, ghostWorldPos.y);
		const glm::ivec2 ghostGridPos = { ghostGridPosPair.first, ghostGridPosPair.second };

		//std::cout << "Ghost Position: " << ghostGridPos.x << ", " << ghostGridPos.y << std::endl;


		// Check if ghost and MsPacman are in the same cell
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
}