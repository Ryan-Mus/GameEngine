#pragma once
#include <Component.h>
#include <Transform.h>
class PacmanGrid;
class PacmanMovement final : public dae::Component
{
public:
    explicit PacmanMovement(float speed, dae::GameObject* owner)
        :Component{ owner }, m_Speed{ speed }
    {
    };

    void Update() override;
    bool SetDirection(const glm::vec3& direction);
    glm::vec3 GetDirection() const { return m_Direction; };
    void SetGrid(PacmanGrid* grid) { m_Grid = grid; };

private:
    glm::vec3 m_Direction{0,0,0}; // Current movement direction
    float m_Speed;
    PacmanGrid* m_Grid{ nullptr };

    // Helper methods
    bool IsAlignedWithGrid() const;
    bool CanMoveInDirection(const glm::vec3& direction);
    void SnapToGridAlignment();
};
