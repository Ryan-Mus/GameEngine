#pragma once
#include <Component.h>
#include <Transform.h>
class GameObject;
class PacmanMovement final: public dae::Component
{
public:
	explicit PacmanMovement(float speed, dae::GameObject* owner)
		:Component{ owner }, m_Speed{speed} {};
	void Update() override;

	void SetDirection(const glm::vec3& direction) { m_Direction = direction; };
	glm::vec3 GetDirection() { return m_Direction; };
private:
	glm::vec3 m_Direction{};
	float m_Speed;
};

