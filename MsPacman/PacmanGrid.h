#pragma once
#include "Component.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "CustomPacmanDefines.h"
#include "MsPacmanSubject.h"
#include "GhostStateComponent.h"
#include "GhostState.h"
#include <vector>
#include <glm.hpp>
#include <iostream>
#include <string>

enum class CellType
{
	Empty = 0,
	Wall = 1,
	Pellet = 2,
	BigPellet = 3,
	GhostHouse = 4,
};

struct Cell
{
	CellType type{ CellType::Empty };
};

class PacmanGrid final : public dae::Component, public MsPacmanSubject
{
public:
	explicit PacmanGrid(dae::GameObject* pOwner, int columns = 28, int rows = 31, float cellSize = 8.f*PACMAN_RENDERSCALE) :
		dae::Component{ pOwner },
		m_Rows{ rows },
		m_Columns{ columns },
		m_CellSize{ static_cast<int>(cellSize) }
	{
		m_Grid.resize(rows, std::vector<Cell>(columns));
		m_Texture = GetOwner()->GetComponent<dae::TextureComponent>()->GetTexture();
	};

	~PacmanGrid() override = default;

	void Render() const override;

	//Top Left corner of the cell
	glm::vec2 GridToWorldPosition(int column, int row) const
	{
		if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
		{
			//Improve for logging
			std::cout << "GridToWorldPosition: Out of bounds" << std::endl;
			return { -1.f, -1.f }; // Out of bounds
		}
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		glm::vec2 worldPos = { pos.x + column * m_CellSize, pos.y + row * m_CellSize };
		return worldPos;
	};

	glm::vec2 GridToLocalPosition(int column, int row) const
	{
		if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
		{
			//Improve for logging
			std::cout << "GridToLocalPosition: Out of bounds" << std::endl;
			return { -1.f, -1.f }; // Out of bounds
		}
		glm::vec2 localPos = { column * m_CellSize, row * m_CellSize };
		return localPos;
	};

	std::pair<int, int> WorldToGridPosition(float x, float y) const
	{
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		int column = static_cast<int>((x - pos.x) / m_CellSize);
		int row = static_cast<int>((y - pos.y) / m_CellSize);

		if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
		{
			//Improve for logging
			std::cout << "WorldToGridPosition: Out of bounds" << std::endl;
			return { -1, -1 }; // Out of bounds
		}
		return { column, row };
	};

	std::pair<int, int> LocalToGridPosition(float x, float y) const
	{
		int column = static_cast<int>(x / m_CellSize);
		int row = static_cast<int>(y / m_CellSize);
		if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
		{
			std::cout << "LocalToGridPosition: Out of bounds" << std::endl;
			return { -1, -1 }; // Out of bounds
		}
		return { column, row };
	};

	void ConsumePellet(int column, int row)
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
			std::cout << "Pellet consumed at (" << column << ", " << row << ")" << std::endl;
		}
		else if (m_Grid[row][column].type == CellType::BigPellet)
		{
			Notify(MsPacmanEvent::EATEN_BIG_PELLET);
			m_Grid[row][column].type = CellType::Empty;
			std::cout << "Big pellet consumed at (" << column << ", " << row << ")" << std::endl;
		}
		else
		{
			std::cout << "No pellet at (" << column << ", " << row << ")" << std::endl;
		}
	}

	CellType GetCellType(int column, int row) const
	{
		if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
		{
			std::cout << "GetCellType: Out of bounds" << std::endl;
			return CellType::Empty; // Out of bounds
		}
		return m_Grid[row][column].type;
	}

	int GetRows() const { return m_Rows; }
	int GetColumns() const { return m_Columns; }
	int GetCellSize() const { return m_CellSize; }

	glm::ivec2 GetDimensions() const { return { m_Columns, m_Rows }; }

	void loadGrid(const std::string& filePath);

	void setMsPacmanPos(int column, int row)
	{
		if (column < 0 || column >= m_Columns || row < 0 || row >= m_Rows)
		{
			std::cout << "SetMsPacmanPos: Out of bounds" << std::endl;
			return; // Out of bounds
		}
		m_MsPacmanPos = { column, row };
	}

	glm::ivec2 GetMsPacmanPos() const
	{
		return m_MsPacmanPos;
	}

	void RegisterMsPacman(dae::GameObject* pMsPacman);
	void RegisterGhost(dae::GameObject* pGhost);

	void Update() override;

private:
	int m_Rows{};
	int m_Columns{};
	int m_CellSize{};
	glm::ivec2 m_MsPacmanPos{ 0, 0 }; // Pacman's position in the grid
	std::vector<std::vector<Cell>> m_Grid;
	dae::Texture2D* m_Texture{ nullptr };
	dae::GameObject* m_pMsPacman{ nullptr };
	std::vector<dae::GameObject*> m_pGhosts{};
};