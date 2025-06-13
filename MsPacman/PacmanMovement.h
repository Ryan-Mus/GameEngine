#pragma once
#include "BaseMovement.h"
class PacmanMovement : public BaseMovement, public MsPacmanObserver
{
public:
	explicit PacmanMovement(float speed, dae::GameObject* owner)
		:BaseMovement{ speed, owner }
	{
	};
	void Update() override;

	void OnNotify(MsPacmanEvent event) override
	{
		if (event == MsPacmanEvent::START_LEVEL)
		{
			m_ElapsedTime = 0.f;
		}
		else if (event == MsPacmanEvent::DIE)
		{
			m_ElapsedTime = -1.f; //Time for respawn
		}
	}
	
private: 
	float m_ElapsedTime{ 0.f };
	float m_StartRoundDuration{ 3.f };
};

