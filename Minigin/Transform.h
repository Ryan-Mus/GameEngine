#pragma once
#include <glm.hpp>

namespace dae
{
	class GameObject;
	class Transform final
	{
	public:
		Transform(GameObject* parent) : m_pParent{ parent } {};

		void SetParent(GameObject* parent) { m_pParent = parent; };

		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() const;

		// Can only change the local position
		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(const glm::vec3 pos);

		void Rotate(float, float, float z);
		void Translate(float x, float y, float z);
		void Scale(float x, float y, float z);

		void UpdateWorldPosition();

		void SetPositionDirty() { m_IsPositionDirty = true; };
		
	private:
		GameObject* m_pParent;

		glm::vec3 m_WorldPosition{};
		glm::vec3 m_LocalPosition{};

		bool m_IsPositionDirty{ false };
	};
}
