#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

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
