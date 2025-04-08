#pragma once
#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>

#include "Transform.h"
#include "Component.h"
#include "Time.h"

namespace dae
{
	class GameObject final
	{
	public:

		//Loop functions
		void Update();
		void FixedUpdate();
		void Render() const;

		//Component functions
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			static_assert(std::is_constructible_v<T, Args..., GameObject*>,
				"T moet constructible zijn met (Args..., GameObject&)");

			std::type_index typeIdx(typeid(T));
			auto component = std::make_unique<T>(std::forward<Args>(args)..., this);
			T& ref = *component;
			components[typeIdx] = std::move(component);
			return ref;
		}

		template<typename T>
		void RemoveComponent() 
		{
			components.erase(std::type_index(typeid(T)));
		}

		template<typename T>
		bool HasComponent() const 
		{
			return components.find(std::type_index(typeid(T))) != components.end();
		}

		template<typename T>
		T* GetComponent()
		{
			auto it = components.find(std::type_index(typeid(T)));
			assert(it != components.end() && "Component not found!");
			return (it != components.end()) ? dynamic_cast<T*>(it->second.get()) : nullptr;
		}

		//Scene graph functions
		void SetParent(GameObject* parent, bool keepWorldPosition = true);
		GameObject* GetParent()				{ return m_pParent; };
		GameObject* GetChildAt(int index);
		size_t GetChildCount()					{ return m_pChildren.size(); };

		//Transform functions
		glm::vec3 GetWorldPosition()	{ return m_Transform.GetWorldPosition(); };
		glm::vec3 GetLocalPosition() { return m_Transform.GetLocalPosition(); };
		void SetLocalPostion(const glm::vec3 pos);
		void SetPositionDirty()
		{
			m_Transform.SetPositionDirty();
			for (uint16_t i{}; i < m_pChildren.size(); ++i)
			{
				m_pChildren[i]->SetPositionDirty();
			}
		};

		void Rotate(float x, float y, float z) { m_Transform.Rotate(x, y, z); };
		void Translate(float x, float y, float z) { m_Transform.Translate(x, y, z); };
		void Scale(float x, float y, float z) { m_Transform.Scale(x, y, z); };


		GameObject() = default;
		GameObject(GameObject* pParent) { SetParent(pParent); }
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		bool IsChild(const GameObject* parent) const;
		bool AddChild(GameObject* child);
		bool RemoveChild(GameObject* child);

		Transform m_Transform{m_pParent};

		GameObject* m_pParent{nullptr};
		std::vector<GameObject*> m_pChildren{};

		std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
	};
}
