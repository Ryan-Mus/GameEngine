#pragma once
#include "Component.h"
#include "MsPacmanObserver.h"
#include "TextureComponent.h"
#include "GameObject.h"

class LivesUIComponent final : public dae::Component, public MsPacmanObserver
{
public:
	LivesUIComponent(dae::GameObject* pOwner):Component(pOwner) 
	{
		m_pTexture = GetOwner()->GetComponent<dae::TextureComponent>()->GetTexture();
	}

	void Render() const override;

	void Update() override;

	void LoseLife();
	void ResetLives();
	int GetLives() const { return m_Lives; }

	void OnNotify(MsPacmanEvent e) override;
private:
	int m_Lives = 3;
	dae::Texture2D* m_pTexture{ nullptr };

	bool m_IsImmune{ false };
	float m_ImmuneTimer{0.f};
	float m_ImmuneDuration{ 2.f };
};


