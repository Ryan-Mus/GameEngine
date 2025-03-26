#pragma once
#include "Transform.h"
#include "GameObject.h"

void dae::Transform::SetLocalPosition(const float x, const float y, const float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
	SetPositionDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec3 pos)
{
	m_LocalPosition = pos;
	SetPositionDirty();
}

const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_IsPositionDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

const glm::vec3& dae::Transform::GetLocalPosition() const
{
	return m_LocalPosition;
}

void dae::Transform::Rotate(float,float,float z)
{
	glm::mat4 rotationMatrix
	{
		{cos(z),-sin(z),0,0},
		{sin(z),cos(z) ,0,0},
		{0	   ,0	   ,1,0},
		{0	   ,0	   ,0,1}
	};

	glm::vec4 newPosition =  rotationMatrix* glm::vec4(m_LocalPosition,1);
	SetLocalPosition(newPosition);
}

//Not implemented yet
void dae::Transform::Translate(float x, float y, float z)
{
	glm::mat4 translateMatrix
	{
		{1,0,0,x},
		{0,1,0,y},
		{0,0,1,z},
		{0,0,0,1}
	};

	glm::vec4 newPosition =glm::vec4(m_LocalPosition, 1)*translateMatrix;
	SetLocalPosition(newPosition);
}

//Not implemented yet
void dae::Transform::Scale(float x, float y, float z)
{
	glm::mat4 scaleMatrix
	{
		{x,0,0,0},
		{0,y,0,0},
		{0,0,z,0},
		{0,0,0,1}
	};

	//m_LocalPosition *= scaleMatrix;
	SetPositionDirty();
}

void dae::Transform::UpdateWorldPosition()
{
	if (m_IsPositionDirty)
	{
		if (m_pParent == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			m_WorldPosition = m_pParent->GetWorldPosition() + m_LocalPosition;
		}
	}
}
