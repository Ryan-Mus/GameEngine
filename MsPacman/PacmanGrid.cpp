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
	while (std::getline(file, line) && row < rows)
	{
		for (int column = 0; column < columns && column < line.size(); ++column)
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
	for (int row = 0; row < rows; ++row)
	{
		for (int column = 0; column < columns; ++column)
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