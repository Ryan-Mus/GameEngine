#pragma once
#include "Component.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "CustomPacmanDefines.h"
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

class PacmanGrid final : public dae::Component
{
public:
	explicit PacmanGrid(dae::GameObject* pOwner, int columns = 28, int rows = 31, float cellSize = 8.f*PACMAN_RENDERSCALE) :
		dae::Component{ pOwner },
		rows{ rows },
		columns{ columns },
		cellSize{ static_cast<int>(cellSize) }
	{
		m_Grid.resize(rows, std::vector<Cell>(columns));
		m_Texture = GetOwner()->GetComponent<dae::TextureComponent>()->GetTexture();
	};

	void Render() const override;

	//Top Left corner of the cell
	glm::vec2 GridToWorldPosition(int column, int row) const
	{
		if (column < 0 || column >= columns || row < 0 || row >= rows)
		{
			//Improve for logging
			std::cout << "GridToWorldPosition: Out of bounds" << std::endl;
			return { -1.f, -1.f }; // Out of bounds
		}
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		glm::vec2 worldPos = { pos.x + column * cellSize, pos.y + row * cellSize };
		return worldPos;
	};

	glm::vec2 GridToLocalPosition(int column, int row) const
	{
		if (column < 0 || column >= columns || row < 0 || row >= rows)
		{
			//Improve for logging
			std::cout << "GridToLocalPosition: Out of bounds" << std::endl;
			return { -1.f, -1.f }; // Out of bounds
		}
		glm::vec2 localPos = { column * cellSize, row * cellSize };
		return localPos;
	};

	std::pair<int, int> WorldToGridPosition(float x, float y) const
	{
		glm::vec3 pos = GetOwner()->GetWorldPosition();
		int column = static_cast<int>((x - pos.x) / cellSize);
		int row = static_cast<int>((y - pos.y) / cellSize);

		if (column < 0 || column >= columns || row < 0 || row >= rows)
		{
			//Improve for logging
			std::cout << "WorldToGridPosition: Out of bounds" << std::endl;
			return { -1, -1 }; // Out of bounds
		}
		return { column, row };
	};

	std::pair<int, int> LocalToGridPosition(float x, float y) const
	{
		int column = static_cast<int>(x / cellSize);
		int row = static_cast<int>(y / cellSize);
		if (column < 0 || column >= columns || row < 0 || row >= rows)
		{
			std::cout << "LocalToGridPosition: Out of bounds" << std::endl;
			return { -1, -1 }; // Out of bounds
		}
		return { column, row };
	};

	void ConsumePellet(int column, int row)
	{
		if (column < 0 || column >= columns || row < 0 || row >= rows)
		{
			std::cout << "ConsumePellet: Out of bounds" << std::endl;
			return; // Out of bounds
		}
		if (m_Grid[row][column].type == CellType::Pellet)
		{
			m_Grid[row][column].type = CellType::Empty;
			std::cout << "Pellet consumed at (" << column << ", " << row << ")" << std::endl;
		}
		else if (m_Grid[row][column].type == CellType::BigPellet)
		{
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
		if (column < 0 || column >= columns || row < 0 || row >= rows)
		{
			std::cout << "GetCellType: Out of bounds" << std::endl;
			return CellType::Empty; // Out of bounds
		}
		return m_Grid[row][column].type;
	}

	int GetRows() const { return rows; }
	int GetColumns() const { return columns; }
	int GetCellSize() const { return cellSize; }

	void loadGrid(const std::string& filePath);

private:
	int rows{};
	int columns{};
	int cellSize{};
	std::vector<std::vector<Cell>> m_Grid;
	dae::Texture2D* m_Texture{ nullptr };
};