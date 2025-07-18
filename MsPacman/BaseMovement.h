#pragma once
#include <Component.h>
#include <Transform.h>
#include "PacmanGrid.h"
class BaseMovement : public dae::Component
{
public:
    explicit BaseMovement(float speed, dae::GameObject* owner)
        :Component{ owner }, m_Speed{ speed }
    {
    };

    void Update() override;
    bool SetDirection(const glm::vec3& direction);
	void SetSpeed(float speed) { m_Speed = speed; };
    glm::vec3 GetDirection() const { return m_Direction; };
    void SetGrid(PacmanGrid* grid) { m_Grid = grid; };
    PacmanGrid* GetGrid() { return m_Grid; };

    void ToggleMovement()
    {
        m_CanMove = !m_CanMove;
        m_Direction = { 0, 0, 0 }; // Stop movement
    }

protected:
    float m_Speed;
    PacmanGrid* m_Grid{ nullptr };
private:
    glm::vec3 m_Direction{0,0,0}; // Current movement direction
    glm::vec3 m_PendingDirection{ 0, 0, 0 };
   
    // Helper methods
    bool IsAlignedWithGrid() const;
    bool CanMoveInDirection(const glm::vec3& direction);
    void SnapToGridAlignment();

	bool m_CanMove{ true }; // Flag to control movement
};
