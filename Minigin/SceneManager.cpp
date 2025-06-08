#include "SceneManager.h"
#include "Scene.h"

namespace dae
{
    Scene& SceneManager::CreateScene(const std::string& name)
    {
        const auto& scene = std::shared_ptr<Scene>(new Scene(name));
        m_scenes.push_back(scene);
        return *scene;
    }

    void SceneManager::SetActiveScene(const std::string& name)
    {
        for (const auto& scene : m_scenes)
        {
            if (scene->GetName() == name)
            {
                m_pActiveScene = scene.get();
                break;
            }
        }
    }

    Scene* SceneManager::GetActiveScene() const
    {
        return m_pActiveScene;
    }

    void SceneManager::Update()
    {
        if (m_pActiveScene)
        {
            m_pActiveScene->Update();
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
        m_scenes.erase(std::remove_if(m_scenes.begin(), m_scenes.end(),
            [&name](const std::shared_ptr<Scene>& scene) { return scene->GetName() == name; }), m_scenes.end());

        if (m_pActiveScene && m_pActiveScene->GetName() == name)
        {
            m_pActiveScene = nullptr;
        }
    }

    void SceneManager::ClearScenes()
    {
        m_scenes.clear();
        m_pActiveScene = nullptr;
    }
}
