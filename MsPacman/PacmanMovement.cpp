#include "PacmanMovement.h"
#include "PacmanGrid.h"

void PacmanMovement::Update()
{
	BaseMovement::Update();
	auto pos = GetOwner()->GetLocalPosition();
	auto gridPos = m_Grid->LocalToGridPosition(pos.x, pos.y);
	auto cellType = m_Grid->GetCellType(gridPos.first, gridPos.second);

	if (cellType == CellType::Pellet)
	{
		m_Grid->ConsumePellet(gridPos.first, gridPos.second);
	}
	else if (cellType == CellType::BigPellet)
	{
		m_Grid->ConsumePellet(gridPos.first, gridPos.second);
	}

	m_Grid->setMsPacmanPos(gridPos.first, gridPos.second);
}
