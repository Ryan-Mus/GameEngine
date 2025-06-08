#include "Scene.h"
#include "GameObject.h" // Include for std::unique_ptr<GameObject>
#include <algorithm>    // For std::remove_if

namespace dae
{
    unsigned int Scene::m_idCounter = 0;

    // Static factory function implementation
    std::unique_ptr<Scene> Scene::Create(const std::string& name)
    {
        return std::unique_ptr<Scene>(new Scene(name));
    }

    Scene::Scene(const std::string& name) : m_name(name) {}

    // Define destructor: Needs full definition of GameObject if m_objects holds unique_ptrs to them.
    Scene::~Scene() = default;

    void Scene::Add(std::unique_ptr<GameObject> object)
    {
        m_objects.emplace_back(std::move(object));
    }

    void Scene::Remove(GameObject* object)
    {
        m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
            [object](const std::unique_ptr<GameObject>& pObject) {
                return pObject.get() == object;
            }), m_objects.end());
    }

    void Scene::RemoveAll()
    {
        m_objects.clear();
    }

    void Scene::Update()
    {
        for(auto& object : m_objects)
        {
            if(object) object->Update();
        }
    }

    void Scene::FixedUpdate()
    {
        for (auto& object : m_objects)
        {
            if(object) object->FixedUpdate();
        }
    }

    void Scene::Render() const
    {
        for (const auto& object : m_objects)
        {
            if(object) object->Render();
        }
    }
}

