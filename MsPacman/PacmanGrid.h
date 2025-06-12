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

struct LevelData
{
	std::string gridFilePath;
	int mazeTextureSourceX;
	int mazeTextureSourceY;
	int mazeTextureSourceWidth;
	int mazeTextureSourceHeight;
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
		m_pTexture = GetOwner()->GetComponent<dae::TextureComponent>()->GetTexture();
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

	void ConsumePellet(int column, int row);

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
	void AddLevelData(const LevelData& levelData);
	void LoadLevel(int levelIndex);

	glm::ivec2 GetMsPacmanPos() const
	{
		return m_MsPacmanPos;
	}

	glm::ivec2 GetPacmanPos() const
	{
		return m_PacmanPos;
	}

	bool IsCoopMode() const
	{
		return m_pPacman != nullptr;
	}

	void RegisterMsPacman(dae::GameObject* pMsPacman);
	void RegisterPacman(dae::GameObject* pPacman);
	void RegisterGhost(dae::GameObject* pGhost);
	void RegisterFruit(dae::GameObject* pFruit);
	void RegisterLives(dae::GameObject* pLives);

	void Update() override;
	void UpdatePacman(dae::GameObject* pacman);

	bool IsScoreForMsPacman() { return m_IsScoreForMsPacman; };


private:
	int m_Rows{};
	int m_Columns{};
	int m_CellSize{};
	glm::ivec2 m_MsPacmanPos{ 0, 0 }; // Pacman's position in the grid
	glm::ivec2 m_PacmanPos{ 0, 0 }; // Pacman's position in the grid
	std::vector<std::vector<Cell>> m_Grid;
	dae::Texture2D* m_pTexture{ nullptr };
	dae::GameObject* m_pMsPacman{ nullptr };
	dae::GameObject* m_pPacman{ nullptr };
	dae::GameObject* m_pFruit{ nullptr };
	dae::GameObject* m_pLives{ nullptr };
	std::vector<dae::GameObject*> m_pGhosts{};

	std::vector<LevelData> m_Levels;
	int m_CurrentLevelIndex = -1;
	int m_TotalPelletsThisLevel = 0;
	int m_PelletsEatenThisLevel = 0;

	bool m_FruitEaten{ false };

	bool m_IsScoreForMsPacman{ true };

	void SetPacmanPos(dae::GameObject* pacman, const glm::ivec2& pos)
	{
		if (pacman)
		{
			if (pacman == m_pMsPacman)
			{
				m_MsPacmanPos = pos;
			}
			else if (pacman == m_pPacman)
			{
				m_PacmanPos = pos;
			}
		}
	}

};