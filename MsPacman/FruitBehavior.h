#pragma once
#include <vector>
#include <glm.hpp>
#include "Component.h"
#include "MsPacmanObserver.h"

enum class FruitType  
{  
	Cherry,  
	Straweberry,  
	Orange  
};  

// Used for changing the fruit after a level complete
inline FruitType operator++(FruitType& fruitType, int) noexcept  
{  
	int next = static_cast<int>(fruitType) + 1;  
	if (next > static_cast<int>(FruitType::Orange))  
		next = static_cast<int>(FruitType::Cherry);  
	fruitType = static_cast<FruitType>(next);  
	return fruitType;  
}

class FruitBehavior final : public dae::Component, public MsPacmanObserver
{
public:
	explicit FruitBehavior(dae::GameObject* owner);

	bool IsSpawned() { return m_IsSpawned; };

private:
	void Update() override;

	void OnNotify(MsPacmanEvent event) override;
 
	bool m_IsEaten{ false };
	bool m_IsSpawned{ false };
	std::vector<glm::ivec2> m_Path;
	size_t m_CurrentPathIndex{ 0 };

	FruitType m_FruitType{ FruitType::Cherry };
};

