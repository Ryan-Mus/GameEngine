#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional> // Include functional for std::function
#include "Singleton.h"
#include "Scene.h" // Include Scene.h for full definition

namespace dae
{
    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        Scene& CreateScene(const std::string& name);

        void SetActiveScene(const std::string& name);
        Scene* GetActiveScene() const;

        bool HasScene(const std::string& name) const;
        void RemoveScene(const std::string& name);
        void ClearScenes();

        void Update();
        void FixedUpdate();
        void Render();

        void QueueSceneChange(std::function<void()> sceneChangeFunc);

    private:
        friend class Singleton<SceneManager>;
        SceneManager();  // Declare constructor
        ~SceneManager(); // Declare destructor

        std::vector<std::unique_ptr<Scene>> m_scenes;
        Scene* m_pActiveScene{ nullptr };
        std::function<void()> m_PendingSceneChange{ nullptr };
    };
}
