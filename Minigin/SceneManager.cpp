#include "SceneManager.h"
#include "InputManager.h"
#include <algorithm>
#include "Scene.h"

namespace dae
{
    SceneManager::SceneManager() = default;
    SceneManager::~SceneManager() = default;

    Scene& SceneManager::CreateScene(const std::string& name)
    {
        auto scene = Scene::Create(name);
        auto& reference = *scene;
        m_scenes.push_back(std::move(scene));
        return reference;
    }

    void SceneManager::SetActiveScene(const std::string& name)
    {
        for (const auto& scene : m_scenes)
        {
            if (scene->GetName() == name)
            {
                m_pActiveScene = scene.get();
                return; // Found and set
            }
        }
    }

    Scene* SceneManager::GetActiveScene() const
    {
        return m_pActiveScene;
    }

    void SceneManager::QueueSceneChange(std::function<void()> sceneChangeFunc)
    {
        m_PendingSceneChange = sceneChangeFunc;
    }

    void SceneManager::Update()
    {
        if (m_pActiveScene)
        {
            m_pActiveScene->Update();
        }

        // Process pending scene change if there is one
        if (m_PendingSceneChange)
        {
            m_PendingSceneChange();
            m_PendingSceneChange = nullptr;
        }
    }

    void SceneManager::FixedUpdate()
    {
        if (m_pActiveScene)
        {
            m_pActiveScene->FixedUpdate();
        }
    }

    void SceneManager::Render()
    {
        if (m_pActiveScene)
        {
            m_pActiveScene->Render();
        }
    }

    bool SceneManager::HasScene(const std::string& name) const
    {
        for (const auto& scene : m_scenes)
        {
            if (scene->GetName() == name)
            {
                return true;
            }
        }
        return false;
    }

    void SceneManager::RemoveScene(const std::string& name)
    {
        bool activeSceneWasRemoved = false;
        if (m_pActiveScene && m_pActiveScene->GetName() == name) {
            activeSceneWasRemoved = true;
        }

        m_scenes.erase(std::remove_if(m_scenes.begin(), m_scenes.end(),
            [&name](const std::unique_ptr<Scene>& scene_ptr) {
                return scene_ptr && scene_ptr->GetName() == name; // Check scene_ptr for safety
            }), m_scenes.end());

        if (activeSceneWasRemoved) {
            m_pActiveScene = m_scenes.empty() ? nullptr : m_scenes.front().get();
        }
        
        // Clean up input bindings associated with this scene
        InputManager::GetInstance().RemoveSceneBindings(name);
    }

    void SceneManager::ClearScenes()
    {
        // Clean up input bindings for all scenes
        for (const auto& scene : m_scenes) {
            if (scene) {
                InputManager::GetInstance().RemoveSceneBindings(scene->GetName());
            }
        }
        
        m_scenes.clear(); // This will call destructors of unique_ptr<Scene>
        m_pActiveScene = nullptr;
    }
}
