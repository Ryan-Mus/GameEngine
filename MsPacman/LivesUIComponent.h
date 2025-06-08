#pragma once
#include "Component.h"
#include "MsPacmanObserver.h"
#include "TextureComponent.h"
#include "GameObject.h"

class LivesUIComponent : public dae::Component, public MsPacmanObserver
{
public:
	LivesUIComponent(dae::GameObject* pOwner):Component(pOwner) 
	{
		m_pTexture = GetOwner()->GetComponent<dae::TextureComponent>()->GetTexture();
	}

	void Render() const override;

	void LoseLife();
	void ResetLives();
	int GetLives() const { return m_Lives; }

	void OnNotify(MsPacmanEvent e) override { if(e == MsPacmanEvent::DIE) LoseLife(); }
private:
	int m_Lives = 3;
	dae::Texture2D* m_pTexture{ nullptr };
};


