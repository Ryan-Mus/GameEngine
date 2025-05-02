#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Singleton.h"

namespace dae
{
    class Scene;
    class SceneManager final : public Singleton<SceneManager>
    {
    public:
        Scene& CreateScene(const std::string& name);

        void SetActiveScene(const std::string& name);
        Scene* GetActiveScene() const;

        void Update();
        void FixedUpdate();
        void Render();
    private:
        friend class Singleton<SceneManager>;
        SceneManager() = default;
        std::vector<std::shared_ptr<Scene>> m_scenes;
        Scene* m_pActiveScene{ nullptr };
    };
}
