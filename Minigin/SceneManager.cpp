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
}
