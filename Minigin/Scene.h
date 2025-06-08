#pragma once
#include <string>
#include <vector>
#include <memory>

namespace dae
{
    class GameObject;

    class Scene final
    {
    public:
        // Static factory function to create Scene
        static std::unique_ptr<Scene> Create(const std::string& name);

        void Add(std::unique_ptr<GameObject> object);
        void Remove(GameObject* object);
        void RemoveAll();

        void Update();
        void FixedUpdate();
        void Render() const;

        std::string GetName() const { return m_name; }
        ~Scene();

        Scene(const Scene& other) = delete;
        Scene(Scene&& other) = delete;
        Scene& operator=(const Scene& other) = delete;
        Scene& operator=(Scene&& other) = delete;

    private:
        // Keep constructor private so only the static factory can call it
        explicit Scene(const std::string& name);

        std::string m_name;
        std::vector<std::unique_ptr<GameObject>> m_objects{};
        static unsigned int m_idCounter;
    };
}