#include "BaseMovement.h"

#include "GameObject.h"
#include "TimeSingleton.h"
#include <iostream>

bool BaseMovement::SetDirection(const glm::vec3& direction)
{
	if (!m_CanMove)
	{
		return false; // Movement is disabled
	}
	if (m_Direction == direction)
	{
		return false; // No change in direction
	}
	if (CanMoveInDirection(direction))
	{
		SnapToGridAlignment();
		m_Direction = direction;
		return true;
	}
	return false;
}

bool BaseMovement::IsAlignedWithGrid() const
{
	auto pos = GetOwner()->GetLocalPosition();
	if (pos.x / m_Grid->GetCellSize() == std::round(pos.x / m_Grid->GetCellSize()) &&
		pos.y / m_Grid->GetCellSize() == std::round(pos.y / m_Grid->GetCellSize()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void BaseMovement::SnapToGridAlignment()
{
	//Snap according the direction
	auto pos = GetOwner()->GetLocalPosition();
	pos.x = std::round(pos.x / m_Grid->GetCellSize()) * m_Grid->GetCellSize();
	pos.y = std::round(pos.y / m_Grid->GetCellSize()) * m_Grid->GetCellSize();

	GetOwner()->SetLocalPostion(pos);
}

//Ugly code, but it works
bool BaseMovement::CanMoveInDirection(const glm::vec3& direction)
{
	auto pos = GetOwner()->GetLocalPosition();
	glm::vec2 nextGridWorldPos{};

	// Calculate center of current cell for more accurate collision detection
	int cellSize = m_Grid->GetCellSize();
	float halfCellSize = cellSize / 2.0f;

	// Start from the center of the current position
	glm::vec2 centerPos(pos.x + halfCellSize, pos.y + halfCellSize);

	// Check ahead in the requested direction
	nextGridWorldPos.x = centerPos.x + direction.x * cellSize;
	nextGridWorldPos.y = centerPos.y + direction.y * cellSize;

	auto nextGridPos = m_Grid->LocalToGridPosition(nextGridWorldPos.x, nextGridWorldPos.y);
	auto cellType = m_Grid->GetCellType(nextGridPos.first, nextGridPos.second);

	if (cellType == CellType::Wall)
	{
		return false; // Can't move into a wall
	}
	else
	{
		return true;
	}
}

void BaseMovement::Update()
{
	auto pos = GetOwner()->GetLocalPosition();
	auto nextPos = pos + m_Direction * m_Speed * dae::Time::GetInstance().GetDeltaTime();


	// Get the center position for pellet detection
	int cellSize = m_Grid->GetCellSize();
	float halfCellSize = m_Grid->GetCellSize() / 2.0f;
	glm::vec2 centerPos(nextPos.x + halfCellSize, nextPos.y + halfCellSize);

	// Use center position for grid position calculation
	auto gridPos = m_Grid->LocalToGridPosition(centerPos.x, centerPos.y);
	auto cellType = m_Grid->GetCellType(gridPos.first, gridPos.second);

	if (pos.x > cellSize * (m_Grid->GetColumns()+1))
	{
		GetOwner()->SetLocalPostion({-halfCellSize,pos.y,0});
		return;
	}

	if (pos.x < -cellSize)
	{
		GetOwner()->SetLocalPostion({ cellSize * (m_Grid->GetColumns() + 0.5f), pos.y, 0 });
		return;
	}

	if (CanMoveInDirection(m_Direction) == false)
	{
		auto currentCell = m_Grid->LocalToGridPosition(pos.x, pos.y);

		//Very ugly
		float epsilon = m_Grid->GetCellSize() * 0.25f;
		glm::vec3 adjustedNextPos = nextPos;

		if (m_Direction.x > 0)
			adjustedNextPos.x -= epsilon;
		if (m_Direction.y > 0)
			adjustedNextPos.y -= epsilon;

		auto nextCell = m_Grid->LocalToGridPosition(adjustedNextPos.x, adjustedNextPos.y);

		if (currentCell != nextCell)
		{
			SnapToGridAlignment();
			return;
		}
	}


	if(cellType ==  CellType::Wall)
	{
		// This shouldn't happen with proper CanMoveInDirection checks
		m_Direction = glm::vec3(0, 0, 0);
		SnapToGridAlignment();
		return;
	}

	GetOwner()->SetLocalPostion(nextPos);
}