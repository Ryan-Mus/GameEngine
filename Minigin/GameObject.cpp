#pragma once
#include <string>
#include <algorithm>

#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

//
//Scene Graph functions
//

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_pParent == parent)
	{
		return;
	}
	if (parent == nullptr)
	{
		m_Transform.SetLocalPosition(m_Transform.GetWorldPosition());
	}
	else
	{
		if (keepWorldPosition)
		{
			m_Transform.SetLocalPosition(m_Transform.GetWorldPosition() - parent->GetWorldPosition());
		}
		SetPositionDirty();
	}
	if (m_pParent) m_pParent->RemoveChild(this);
	m_pParent = parent;
	m_Transform.SetParent(parent);
	if (m_pParent) m_pParent->AddChild(this);
}

//
//Transform
//

void dae::GameObject::SetLocalPostion(const glm::vec3 pos)
{
	m_Transform.SetLocalPosition(pos);
	SetPositionDirty();
}

dae::GameObject::~GameObject()
{
	for (auto child : m_pChildren)
	{
		child->SetParent(nullptr);
	}
}

//
// loop functions
//

void dae::GameObject::Update()
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		it->second->Update();
	}

}

void dae::GameObject::FixedUpdate()
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		it->second->FixedUpdate();
	}

}

void dae::GameObject::Render() const
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		it->second->Render();
	}
}

//
// Child functions
//

dae::GameObject* dae::GameObject::GetChildAt(int index)
{
	if (index >= 0 && static_cast<size_t>(index) < m_pChildren.size()) return m_pChildren[index];
	else return nullptr;
}

bool dae::GameObject::IsChild(const GameObject* parent) const
{
	if (!parent) return false;

	GameObject* current = m_pParent;
	while (current)
	{
		if (current == parent) return true;
		current = current->m_pParent;
	}
	return false;
}

bool dae::GameObject::AddChild(GameObject* child)
{
	m_pChildren.emplace_back(child);
	return true;
}

bool dae::GameObject::RemoveChild(GameObject* child)
{
	auto it = std::find(m_pChildren.begin(), m_pChildren.end(),child);

	if (it != m_pChildren.end()) {
		m_pChildren.erase(it);
		return true;
	}
	return false;
}
