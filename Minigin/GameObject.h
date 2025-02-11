#pragma once
#include <memory>
#include "Transform.h"
#include "Component.h"
#include "Time.h"
#include <vector>
#include <typeindex>
#include <unordered_map>

namespace dae
{
	class GameObject final
	{
	public:
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render() const;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			std::type_index typeIdx(typeid(T));
			auto component = std::make_unique<T>(std::forward<Args>(args)...);
			T& ref = *component;
			components[typeIdx] = std::move(component);
			return ref;
		}

		template<typename T>
		void RemoveComponent() {
			components.erase(std::type_index(typeid(T)));
		}

		template<typename T>
		bool HasComponent() const {
			return components.find(std::type_index(typeid(T))) != components.end();
		}

		template<typename T>
		T* GetComponent() {
			auto it = components.find(std::type_index(typeid(T)));
			return (it != components.end()) ? dynamic_cast<T*>(it->second.get()) : nullptr;
		}

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
	};
}
